from flask_restx import Namespace, Resource, fields
from flask_jwt_extended import jwt_required, get_jwt_identity
from app.services.facade import facade

profile_namespace = Namespace('profile', description='User profile and preferences')

# Models for Swagger
profile_model = profile_namespace.model('UserProfile', {
    'selected_character': fields.Integer(required=False, description='Selected character (1-8)', default=1),
    'highest_level_reached': fields.Integer(required=False, description='Highest level reached', default=1),
    'total_playtime': fields.Float(required=False, description='Total playtime in seconds', default=0.0),
    'unlocked_worlds': fields.String(required=False, description='Comma-separated unlocked worlds', default='Space')
})

profile_update_model = profile_namespace.model('ProfileUpdate', {
    'selected_character': fields.Integer(required=False, description='Selected character (1-8)'),
    'total_playtime': fields.Float(required=False, description='Total playtime in seconds')
})

world_unlock_model = profile_namespace.model('WorldUnlock', {
    'world': fields.String(required=True, description='World to unlock (Space, Ocean, Desert, Forest, SpaceShip)')
})

@profile_namespace.route('/me')
class MyProfile(Resource):
    @jwt_required()
    @profile_namespace.response(200, 'Success')
    @profile_namespace.response(404, 'Profile not found')
    def get(self):
        """Get current user's profile"""
        current_user_id = get_jwt_identity()
        profile = facade.get_user_profile_by_user(current_user_id)
        
        if not profile:
            profile = facade.create_user_profile({
                'user_id': current_user_id,
                'selected_character': 0,
                'highest_level_reached': 1,
                'total_playtime': 0.0,
                'unlocked_worlds': 'Space'
            })

        user = facade.get_user(current_user_id)
        result = profile.to_dict()
        result['user'] = {
            'id': user.id,
            'first_name': user.first_name,
            'last_name': user.last_name,
            'email': user.email
        }
        
        return result, 200
    
    @jwt_required()
    @profile_namespace.expect(profile_update_model, validate=True)
    @profile_namespace.response(200, 'Profile updated successfully')
    @profile_namespace.response(404, 'Profile not found')
    def put(self):
        """Update current user's profile"""
        current_user_id = get_jwt_identity()
        profile = facade.get_user_profile_by_user(current_user_id)
        
        if not profile:
            return {'error': 'Profile not found. Create profile first.'}, 404
        
        try:
            updated_profile = facade.update_user_profile(profile.id, profile_namespace.payload)
            return updated_profile.to_dict(), 200
        except ValueError as e:
            return {'error': str(e)}, 400

@profile_namespace.route('/me/unlock-world')
class UnlockWorld(Resource):
    @jwt_required()
    @profile_namespace.expect(world_unlock_model, validate=True)
    @profile_namespace.response(200, 'World unlocked successfully')
    @profile_namespace.response(400, 'Invalid world')
    def post(self):
        """Unlock a new world for the user"""
        current_user_id = get_jwt_identity()
        profile = facade.get_user_profile_by_user(current_user_id)
        
        if not profile:
            profile = facade.create_user_profile({
                'user_id': current_user_id,
                'selected_character': 0,
                'highest_level_reached': 1,
                'total_playtime': 0.0,
                'unlocked_worlds': 'Space'
            })
        
        world = profile_namespace.payload.get('world')
        valid_worlds = ['Space', 'Ocean', 'Desert', 'Forest', 'SpaceShip']
        
        if world not in valid_worlds:
            return {'error': f'Invalid world. Must be one of: {valid_worlds}'}, 400
        
        profile.unlock_world(world)
        from app import db
        db.session.commit()
        
        return {
            'message': f'World {world} unlocked',
            'unlocked_worlds': profile.get_unlocked_worlds()
        }, 200

@profile_namespace.route('/me/stats')
class ProfileStats(Resource):
    @jwt_required()
    @profile_namespace.response(200, 'Success')
    def get(self):
        """Get detailed statistics for current user"""
        current_user_id = get_jwt_identity()

        profile = facade.get_user_profile_by_user(current_user_id)
        if not profile:
            profile = facade.create_user_profile({
                'user_id': current_user_id,
                'selected_character': 0,
                'highest_level_reached': 1,
                'total_playtime': 0.0,
                'unlocked_worlds': 'Space'
            })

        completions = facade.get_user_completions(current_user_id)

        scores = facade.get_scores_by_user(current_user_id)

        total_levels_completed = len(completions)
        total_score = sum(s.value for s in scores)
        best_score = max([s.value for s in scores], default=0)

        worlds_progress = {}
        for comp in completions:
            if comp.world not in worlds_progress:
                worlds_progress[comp.world] = {
                    'levels_completed': 0,
                    'best_score': 0,
                    'total_time': 0
                }
            worlds_progress[comp.world]['levels_completed'] += 1
            worlds_progress[comp.world]['best_score'] = max(
                worlds_progress[comp.world]['best_score'],
                comp.best_score
            )
            worlds_progress[comp.world]['total_time'] += comp.best_time
        
        return {
            'profile': profile.to_dict(),
            'total_levels_completed': total_levels_completed,
            'total_score': total_score,
            'best_score': best_score,
            'worlds_progress': worlds_progress,
            'recent_scores': [s.to_dict() for s in scores[-5:]]
        }, 200
