from flask_restx import Namespace, Resource, fields
from flask_jwt_extended import create_access_token, jwt_required, get_jwt_identity
from app.services.facade import facade

api = Namespace('auth', description='Authentication operations')

login_model = api.model('Login', {
    'email': fields.String(required=False, description='User email'),
    'username': fields.String(required=False, description='Username (email)'),
    'password': fields.String(required=True, description='User password')
})

@api.route('/login')
class Login(Resource):
    @api.expect(login_model)
    def post(self):
        """Authenticate user and return a JWT token"""
        credentials = api.payload
        
        # Accept either 'email' or 'username' field
        email = credentials.get('email') or credentials.get('username')
        password = credentials.get('password')
        
        if not email or not password:
            return {'error': 'Email/username and password are required'}, 400
        
        user = facade.get_user_by_email(email)

        if user and user.verify_password(password):
            access_token = create_access_token(
                identity=str(user.id),
                additional_claims={"is_admin": user.is_admin}
            )
            return {'access_token': access_token}, 200
        
        return {'error': 'Invalid credentials'}, 401

@api.route('/protected')
class ProtectedResource(Resource):
    @jwt_required()
    def get(self):
        current_user = get_jwt_identity()
        return {'message': f'Hello, user {current_user}'}, 200

init_admin_model = api.model('InitAdmin', {
    'username': fields.String(required=False, description='Admin username (used for first/last name)'),
    'email': fields.String(required=True, description='Admin email'),
    'password': fields.String(required=True, description='Admin password')
})

@api.route('/init-admin')
class InitAdmin(Resource):
    @api.expect(init_admin_model)
    def post(self):
        """Create initial admin user (à utiliser une seule fois)"""
        try:
            payload = api.payload or {}
            email = payload.get('email', 'admin@si3ln.com')
            password = payload.get('password', 'admin123')
            username = payload.get('username', 'Admin')
            
            existing_admin = facade.get_user_by_email(email)
            if existing_admin:
                return {'error': 'Admin user already exists'}, 400

            admin_data = {
                "first_name": username,
                "last_name": "User", 
                "email": email,
                "password": password,
                "is_admin": True
            }
            
            admin = facade.create_user(admin_data)
            return {
                'message': 'Admin user created successfully',
                'user': {
                    'id': admin.id,
                    'email': admin.email
                }
            }, 201
            
        except Exception as e:
            return {'error': f'Failed to create admin: {str(e)}'}, 500
