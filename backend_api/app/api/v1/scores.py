from flask_restx import Namespace, Resource, fields
from flask_jwt_extended import jwt_required, get_jwt_identity
from app.services.facade import facade

score_namespace = Namespace('scores', description='Score operations')

# Models for Swagger documentation
score_model = score_namespace.model('Score', {
    'player_name': fields.String(required=True, description='Player name'),
    'value': fields.Integer(required=True, description='Score value'),
    'level': fields.Integer(required=True, description='Level achieved (1-10)'),
    'game_id': fields.String(required=False, description='Associated game ID')
})

@score_namespace.route('/')
class ScoreList(Resource):
    @jwt_required()
    @score_namespace.expect(score_model, validate=True)
    @score_namespace.response(201, 'Score successfully created')
    @score_namespace.response(400, 'Invalid input data')
    def post(self):
        """Submit a new score"""
        current_user_id = get_jwt_identity()
        score_data = score_namespace.payload
        score_data['user_id'] = current_user_id
        
        try:
            new_score = facade.create_score(score_data)
            return new_score.to_dict(), 201
        except ValueError as e:
            return {'error': str(e)}, 400
        except Exception as e:
            return {'error': f'Internal server error: {str(e)}'}, 500
    
    @score_namespace.response(200, 'Success')
    @score_namespace.param('limit', 'Number of scores to return', type='integer', default=10)
    @score_namespace.param('level', 'Filter by level', type='integer')
    @score_namespace.param('world', 'Filter by world', type='string')
    def get(self):
        """Get high scores (leaderboard)"""
        from flask import request
        limit = request.args.get('limit', 10, type=int)
        level = request.args.get('level', type=int)
        world = request.args.get('world', type=str)
        
        try:
            # If world is specified, get scores from games in that world
            if world:
                from app import db
                from app.models.game import Game
                
                # Get scores for games in the specified world
                scores = db.session.query(Score).join(Game).filter(
                    Game.world == world
                )
                
                if level:
                    scores = scores.filter(Score.level == level)
                
                scores = scores.order_by(Score.value.desc()).limit(limit).all()
            else:
                scores = facade.get_top_scores(limit=limit, level=level)
            
            return [score.to_dict() for score in scores], 200
        except Exception as e:
            return {'error': str(e)}, 500

@score_namespace.route('/<string:score_id>')
class ScoreResource(Resource):
    @jwt_required()
    @score_namespace.response(200, 'Success')
    @score_namespace.response(404, 'Score not found')
    def get(self, score_id):
        """Get score details by ID"""
        score = facade.get_score(score_id)
        
        if not score:
            return {'error': 'Score not found'}, 404
        
        return score.to_dict(), 200
    
    @jwt_required()
    @score_namespace.response(204, 'Score successfully deleted')
    @score_namespace.response(404, 'Score not found')
    @score_namespace.response(403, 'Not authorized')
    def delete(self, score_id):
        """Delete a score (user's own or admin)"""
        current_user_id = get_jwt_identity()
        current_user = facade.get_user(current_user_id)
        score = facade.get_score(score_id)
        
        if not score:
            return {'error': 'Score not found'}, 404
        
        if score.user_id != current_user_id and not current_user.is_admin:
            return {'error': 'Not authorized to delete this score'}, 403
        
        facade.delete_score(score_id)
        return '', 204

@score_namespace.route('/user/<string:user_id>')
class UserScores(Resource):
    @jwt_required()
    @score_namespace.response(200, 'Success')
    @score_namespace.response(404, 'User not found')
    def get(self, user_id):
        """Get all scores for a specific user"""
        user = facade.get_user(user_id)
        
        if not user:
            return {'error': 'User not found'}, 404
        
        scores = facade.get_scores_by_user(user_id)
        return [score.to_dict() for score in scores], 200

@score_namespace.route('/my-scores')
class MyScores(Resource):
    @jwt_required()
    @score_namespace.response(200, 'Success')
    def get(self):
        """Get current user's scores"""
        current_user_id = get_jwt_identity()
        scores = facade.get_scores_by_user(current_user_id)
        return [score.to_dict() for score in scores], 200

@score_namespace.route('/game/<string:game_id>')
class GameScores(Resource):
    @jwt_required()
    @score_namespace.response(200, 'Success')
    @score_namespace.response(404, 'Game not found')
    def get(self, game_id):
        """Get all scores for a specific game"""
        game = facade.get_game(game_id)
        
        if not game:
            return {'error': 'Game not found'}, 404
        
        scores = facade.get_scores_by_game(game_id)
        return [score.to_dict() for score in scores], 200

@score_namespace.route('/leaderboard/level/<int:level>')
class LevelLeaderboard(Resource):
    @score_namespace.response(200, 'Success')
    @score_namespace.param('limit', 'Number of scores to return', type='integer', default=10)
    def get(self, level):
        """Get leaderboard for a specific level"""
        from flask import request
        limit = request.args.get('limit', 10, type=int)
        
        if level < 1 or level > 10:
            return {'error': 'Level must be between 1 and 10'}, 400
        
        try:
            scores = facade.get_top_scores(limit=limit, level=level)
            return [score.to_dict() for score in scores], 200
        except Exception as e:
            return {'error': str(e)}, 500

@score_namespace.route('/stats')
class ScoreStats(Resource):
    @jwt_required()
    @score_namespace.response(200, 'Success')
    def get(self):
        """Get current user's score statistics"""
        current_user_id = get_jwt_identity()
        scores = facade.get_scores_by_user(current_user_id)
        
        if not scores:
            return {
                'total_scores': 0,
                'highest_score': 0,
                'average_score': 0,
                'levels_completed': []
            }, 200
        
        total_scores = len(scores)
        highest_score = max(score.value for score in scores)
        average_score = sum(score.value for score in scores) / total_scores
        levels_completed = sorted(list(set(score.level for score in scores)))
        
        return {
            'total_scores': total_scores,
            'highest_score': highest_score,
            'average_score': round(average_score, 2),
            'levels_completed': levels_completed
        }, 200
