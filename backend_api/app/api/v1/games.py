from flask_restx import Namespace, Resource, fields
from flask_jwt_extended import jwt_required, get_jwt_identity
from app.services.facade import facade

game_namespace = Namespace('games', description='Game operations')

# Models for Swagger documentation
game_start_model = game_namespace.model('GameStart', {
    'player_name': fields.String(required=True, description='Player name'),
    'level': fields.Integer(required=False, default=1, description='Game level (1-5)'),
    'world': fields.String(required=False, default='Space', description='Game world (Space, Ocean, Desert, Forest, City)')
})

game_update_model = game_namespace.model('GameUpdate', {
    'score': fields.Integer(required=False, description='Current score'),
    'status': fields.String(required=False, description='Game status (active, completed, game_over)'),
    'time_elapsed': fields.Float(required=False, description='Time elapsed in seconds'),
    'level': fields.Integer(required=False, description='Current level')
})

level_selection_model = game_namespace.model('LevelSelection', {
    'level': fields.Integer(required=True, description='Level to start (1-5)'),
    'world': fields.String(required=False, default='Space', description='World selection')
})

@game_namespace.route('/')
class GameList(Resource):
    @jwt_required()
    @game_namespace.expect(game_start_model, validate=True)
    @game_namespace.response(201, 'Game successfully created')
    @game_namespace.response(400, 'Invalid input data')
    def post(self):
        """Start a new game"""
        current_user_id = get_jwt_identity()
        game_data = game_namespace.payload
        game_data['user_id'] = current_user_id
        
        try:
            new_game = facade.create_game(game_data)
            return new_game.to_dict(), 201
        except ValueError as e:
            return {'error': str(e)}, 400
        except Exception as e:
            return {'error': f'Internal server error: {str(e)}'}, 500
    
    @jwt_required()
    @game_namespace.response(200, 'Success')
    def get(self):
        """Get all games for current user"""
        current_user_id = get_jwt_identity()
        games = facade.get_games_by_user(current_user_id)
        return [game.to_dict() for game in games], 200

@game_namespace.route('/<string:game_id>')
class GameResource(Resource):
    @jwt_required()
    @game_namespace.response(200, 'Success')
    @game_namespace.response(404, 'Game not found')
    @game_namespace.response(403, 'Not authorized')
    def get(self, game_id):
        """Get game details by ID"""
        current_user_id = get_jwt_identity()
        game = facade.get_game(game_id)
        
        if not game:
            return {'error': 'Game not found'}, 404
        
        if game.user_id != current_user_id:
            return {'error': 'Not authorized to view this game'}, 403
        
        return game.to_dict(), 200
    
    @jwt_required()
    @game_namespace.expect(game_update_model, validate=True)
    @game_namespace.response(200, 'Game successfully updated')
    @game_namespace.response(404, 'Game not found')
    @game_namespace.response(403, 'Not authorized')
    def put(self, game_id):
        """Update game state"""
        current_user_id = get_jwt_identity()
        game = facade.get_game(game_id)
        
        if not game:
            return {'error': 'Game not found'}, 404
        
        if game.user_id != current_user_id:
            return {'error': 'Not authorized to update this game'}, 403
        
        try:
            updated_game = facade.update_game(game_id, game_namespace.payload)
            return updated_game.to_dict(), 200
        except ValueError as e:
            return {'error': str(e)}, 400
    
    @jwt_required()
    @game_namespace.response(204, 'Game successfully deleted')
    @game_namespace.response(404, 'Game not found')
    @game_namespace.response(403, 'Not authorized')
    def delete(self, game_id):
        """Delete a game"""
        current_user_id = get_jwt_identity()
        game = facade.get_game(game_id)
        
        if not game:
            return {'error': 'Game not found'}, 404
        
        if game.user_id != current_user_id:
            return {'error': 'Not authorized to delete this game'}, 403
        
        facade.delete_game(game_id)
        return '', 204

@game_namespace.route('/levels/available')
class AvailableLevels(Resource):
    @jwt_required()
    @game_namespace.response(200, 'Success')
    def get(self):
        """Get available levels and worlds"""
        levels = [
            {'level': i, 'name': f'Level {i}', 'difficulty': 'Easy' if i <= 2 else 'Medium' if i <= 3 else 'Hard'}
            for i in range(1, 11)
        ]
        worlds = ['Space', 'Ocean', 'Desert', 'Forest', 'City']
        
        return {
            'levels': levels,
            'worlds': worlds
        }, 200

@game_namespace.route('/active')
class ActiveGame(Resource):
    @jwt_required()
    @game_namespace.response(200, 'Success')
    @game_namespace.response(404, 'No active game found')
    def get(self):
        """Get current user's active game"""
        current_user_id = get_jwt_identity()
        game = facade.get_active_game(current_user_id)
        
        if not game:
            return {'error': 'No active game found'}, 404
        
        return game.to_dict(), 200

@game_namespace.route('/<string:game_id>/complete')
class CompleteGame(Resource):
    @jwt_required()
    @game_namespace.response(200, 'Game completed')
    @game_namespace.response(404, 'Game not found')
    @game_namespace.response(403, 'Not authorized')
    def post(self, game_id):
        """Mark game as completed and record results"""
        current_user_id = get_jwt_identity()
        game = facade.get_game(game_id)
        
        if not game:
            return {'error': 'Game not found'}, 404
        
        if game.user_id != current_user_id:
            return {'error': 'Not authorized'}, 403
        
        try:
            updated_game = facade.update_game(game_id, {'status': 'completed'})

            score_data = {
                'player_name': game.player_name,
                'value': game.score,
                'level': game.level,
                'user_id': current_user_id,
                'game_id': game_id
            }
            score = facade.create_score(score_data)
            completion = facade.record_level_completion(
                user_id=current_user_id,
                world=game.world,
                level=game.level,
                score=game.score,
                time_elapsed=game.time_elapsed
            )
            
            return {
                'game': updated_game.to_dict(),
                'score': score.to_dict(),
                'completion': completion.to_dict()
            }, 200
        except Exception as e:
            return {'error': str(e)}, 500

@game_namespace.route('/by-world/<string:world>')
class GamesByWorld(Resource):
    @jwt_required()
    @game_namespace.response(200, 'Success')
    def get(self, world):
        """Get all games for current user in a specific world"""
        current_user_id = get_jwt_identity()
        from app import db
        from app.models.game import Game
        
        games = db.session.query(Game).filter_by(
            user_id=current_user_id,
            world=world
        ).order_by(Game.created_at.desc()).all()
        
        return [game.to_dict() for game in games], 200
    def post(self, game_id):
        """Mark game as completed"""
        current_user_id = get_jwt_identity()
        game = facade.get_game(game_id)
        
        if not game:
            return {'error': 'Game not found'}, 404
        
        if game.user_id != current_user_id:
            return {'error': 'Not authorized'}, 403
        
        try:
            updated_game = facade.update_game(game_id, {'status': 'completed'})
            return updated_game.to_dict(), 200
        except Exception as e:
            return {'error': str(e)}, 500
