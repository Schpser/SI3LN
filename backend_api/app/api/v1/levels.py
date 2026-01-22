from flask_restx import Namespace, Resource, fields
from flask_jwt_extended import jwt_required, get_jwt_identity
from app.services.facade import facade

levels_namespace = Namespace('levels', description='Level completion tracking')

# Models for Swagger documentation
level_completion_model = levels_namespace.model('LevelCompletion', {
    'world': fields.String(required=True, description='World name'),
    'level': fields.Integer(required=True, description='Level number (1-10)'),
    'score': fields.Integer(required=True, description='Score achieved'),
    'time_elapsed': fields.Float(required=True, description='Time taken in seconds')
})

@levels_namespace.route('/complete')
class CompleteLevelEndpoint(Resource):
    @jwt_required()
    @levels_namespace.expect(level_completion_model, validate=True)
    @levels_namespace.response(201, 'Level completion recorded')
    @levels_namespace.response(400, 'Invalid input')
    def post(self):
        """Record a level completion and unlock next level"""
        current_user_id = get_jwt_identity()
        data = levels_namespace.payload
        
        world = data.get('world')
        level = data.get('level')
        score = data.get('score')
        time_elapsed = data.get('time_elapsed')
        
        try:
            # Record the completion
            completion = facade.record_level_completion(
                user_id=current_user_id,
                world=world,
                level=level,
                score=score,
                time_elapsed=time_elapsed
            )
            
            # Update user profile's highest level
            profile = facade.get_user_profile_by_user(current_user_id)
            if not profile:
                profile = facade.create_user_profile({
                    'user_id': current_user_id,
                    'selected_character': 0,
                    'highest_level_reached': level,
                    'total_playtime': 0.0,
                    'unlocked_worlds': world
                })
            else:
                if level > profile.highest_level_reached:
                    facade.update_user_profile(profile.id, {
                        'highest_level_reached': level
                    })
                
                # Ensure world is unlocked
                if not profile.is_world_unlocked(world):
                    profile.unlock_world(world)
                    from app import db
                    db.session.commit()

            next_level_unlocked = False
            next_world_unlocked = None
            
            if level < 5:
                next_level_unlocked = True
            elif level == 5:
                world_order = ['Space', 'Ocean', 'Desert', 'Forest', 'Spaceship']
                try:
                    current_idx = world_order.index(world)
                    if current_idx < len(world_order) - 1:
                        next_world = world_order[current_idx + 1]
                        profile.unlock_world(next_world)
                        from app import db
                        db.session.commit()
                        next_world_unlocked = next_world
                except (ValueError, IndexError):
                    pass
            
            return {
                'completion': completion.to_dict(),
                'next_level_unlocked': next_level_unlocked,
                'next_world_unlocked': next_world_unlocked,
                'is_new_best': score >= completion.best_score
            }, 201
            
        except ValueError as e:
            return {'error': str(e)}, 400
        except Exception as e:
            return {'error': f'Internal server error: {str(e)}'}, 500

@levels_namespace.route('/my-completions')
class MyCompletions(Resource):
    @jwt_required()
    @levels_namespace.param('world', 'Filter by world', type='string')
    @levels_namespace.response(200, 'Success')
    def get(self):
        """Get all level completions for current user"""
        from flask import request
        current_user_id = get_jwt_identity()
        world = request.args.get('world')
        
        completions = facade.get_user_completions(current_user_id, world)
        return [c.to_dict() for c in completions], 200

@levels_namespace.route('/my-completions/<string:world>/<int:level>')
class SpecificCompletion(Resource):
    @jwt_required()
    @levels_namespace.response(200, 'Success')
    @levels_namespace.response(404, 'Completion not found')
    def get(self, world, level):
        """Get specific level completion details"""
        current_user_id = get_jwt_identity()
        completion = facade.get_user_level_completion(current_user_id, world, level)
        
        if not completion:
            return {'error': 'Level not yet completed'}, 404
        
        return completion.to_dict(), 200

@levels_namespace.route('/available')
class AvailableLevels(Resource):
    @jwt_required()
    @levels_namespace.response(200, 'Success')
    def get(self):
        """Get available/unlocked levels and worlds for current user"""
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

        unlocked_worlds = profile.get_unlocked_worlds()
        world_levels = {}
        
        for world in unlocked_worlds:
            completed_levels = [c.level for c in completions if c.world == world]
            max_completed = max(completed_levels, default=0)
            
            world_levels[world] = {
                'unlocked': True,
                'completed_levels': sorted(completed_levels),
                'available_levels': list(range(1, min(max_completed + 2, 11))),  # Next level unlocked
                'total_levels': 5
            }
        
        return {
            'unlocked_worlds': unlocked_worlds,
            'world_levels': world_levels,
            'highest_level_reached': profile.highest_level_reached
        }, 200

@levels_namespace.route('/leaderboard/<string:world>/<int:level>')
class LevelLeaderboard(Resource):
    @levels_namespace.response(200, 'Success')
    @levels_namespace.param('limit', 'Number of results', type='integer', default=10)
    def get(self, world, level):
        """Get leaderboard for a specific world and level"""
        from flask import request
        from app import db
        from app.models.level_completion import LevelCompletion
        
        limit = request.args.get('limit', 10, type=int)
        
        # Get top completions for this world/level
        top_completions = db.session.query(LevelCompletion).filter_by(
            world=world,
            level=level
        ).order_by(LevelCompletion.best_score.desc()).limit(limit).all()
        
        # Include user names
        results = []
        for comp in top_completions:
            user = facade.get_user(comp.user_id)
            results.append({
                'rank': len(results) + 1,
                'player_name': f"{user.first_name} {user.last_name}",
                'score': comp.best_score,
                'time': comp.best_time,
                'completed_at': comp.completed_at.isoformat()
            })
        
        return {
            'world': world,
            'level': level,
            'leaderboard': results
        }, 200
