from flask import Flask
from flask_cors import CORS
from flask_bcrypt import Bcrypt
from flask_restx import Api
from flask_jwt_extended import JWTManager
from flask_sqlalchemy import SQLAlchemy

bcrypt = Bcrypt()
jwt = JWTManager()
db = SQLAlchemy()

authorizations = {
    'Bearer Auth': {
        'type': 'apiKey',
        'in': 'header',
        'name': 'Authorization',
        'description': 'Enter: Bearer <your_token>'
    }
}

def create_app():
    app = Flask(__name__)
    CORS(app)
    app.config.from_object('config.DevelopmentConfig')

    api = Api(app, 
              version='1.0', 
              title='Game API', 
              description='Space Invaders Game API', 
              doc='/',
              authorizations=authorizations,
              security='Bearer Auth',
              serve_challenge_on_401=True)

    bcrypt.init_app(app)
    jwt.init_app(app)
    db.init_app(app)

    from app.models import User, Game, Score, UserProfile, LevelCompletion

    with app.app_context():
        db.create_all()
        print("✅ Tables créées avec succès!")

    from app.api.v1.users import user_namespace as users_ns
    from app.api.v1.games import game_namespace
    from app.api.v1.scores import score_namespace
    from app.api.v1.auth import api as auth_ns
    from app.api.v1.profile import profile_namespace
    from app.api.v1.levels import levels_namespace

    api.add_namespace(users_ns, path='/api/v1/users')
    api.add_namespace(auth_ns, path='/api/v1/auth')
    api.add_namespace(game_namespace, path='/api/v1/games')
    api.add_namespace(score_namespace, path='/api/v1/scores')
    api.add_namespace(profile_namespace, path='/api/v1/profile')
    api.add_namespace(levels_namespace, path='/api/v1/levels')
    
    return app