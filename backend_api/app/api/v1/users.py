from flask_restx import Namespace, Resource, fields
from flask_jwt_extended import jwt_required, get_jwt_identity
from app.services.facade import facade

user_namespace = Namespace('users', description='User operations')

user_model = user_namespace.model('User', {
    'first_name': fields.String(required=True, description='First name of the user'),
    'last_name': fields.String(required=True, description='Last name of the user'),
    'email': fields.String(required=True, description='Email of the user'),
    'password': fields.String(required=True, description='Password of the user')
})

user_update_model = user_namespace.model('UserUpdate', {
    'first_name': fields.String(required=False, description='First name of the user'),
    'last_name': fields.String(required=False, description='Last name of the user'),
    'email': fields.String(required=False, description='Email of the user'),
    'password': fields.String(required=False, description='Password of the user')
})

@user_namespace.route('/')
class UserList(Resource):
    @user_namespace.expect(user_model, validate=True)
    @jwt_required()
    @user_namespace.response(201, 'User successfully created')
    @user_namespace.response(400, 'Email already registered') 
    @user_namespace.response(400, 'Invalid input data')
    @user_namespace.response(403, 'Admin privileges required')
    def post(self):
        """Register a new user (Admin only)"""
        
        current_user_id = get_jwt_identity()
        current_user = facade.get_user(current_user_id)

        if not current_user or not current_user.is_admin:
            return {'error': 'Admin privileges required'}, 403

        user_data = user_namespace.payload

        existing_user = facade.get_user_by_email(user_data['email'])
        if existing_user:
            return {'error': 'Email already registered'}, 400
        
        try:
            user_data_copy = user_data.copy()
            password = user_data_copy.pop('password')
            new_user = facade.create_user(user_data_copy)
            new_user.hash_password(password)
            facade.update_user(new_user.id, {'password': new_user.password})
            
            return {
                'id': new_user.id, 
                'first_name': new_user.first_name, 
                'last_name': new_user.last_name, 
                'email': new_user.email
            }, 201
        except ValueError as e:
            return {'error': str(e)}, 400
        except Exception as e:
            return {'error': f'Internal server error: {str(e)}'}, 500
    
    @jwt_required()
    def get(self):
        """Get all users"""
        users = facade.get_all_users()
        return [{
            'id': user.id,
            'first_name': user.first_name,
            'last_name': user.last_name,
            'email': user.email
        } for user in users], 200

@user_namespace.route('/<string:user_id>')
class UserResource(Resource):
    @jwt_required()
    def get(self, user_id):
        """Get user details by ID"""
        user = facade.get_user(user_id)
        if not user:
            return {'error': 'User not found'}, 404
        return {
            'id': user.id, 
            'first_name': user.first_name, 
            'last_name': user.last_name, 
            'email': user.email
        }, 200

    @user_namespace.expect(user_update_model, validate=True)
    @jwt_required()
    @user_namespace.response(200, 'User successfully updated')
    @user_namespace.response(404, 'User not found')
    @user_namespace.response(400, 'Email already registered')
    def put(self, user_id):
        """Update a user"""
        current_user_id = get_jwt_identity()
        current_user = facade.get_user(current_user_id)
        existing_user = facade.get_user(user_id)

        if not existing_user:
            return {'error': 'User not found'}, 404

        if not current_user or (not current_user.is_admin and current_user_id != user_id):
            return {'error': 'Admin privileges required or you can only update your own profile'}, 403

        user_data = user_namespace.payload

        if 'password' in user_data:
            existing_user.hash_password(user_data['password'])
            user_data['password'] = existing_user.password

        if 'email' in user_data and user_data['email'] != existing_user.email:
            user_with_email = facade.get_user_by_email(user_data['email'])
            if user_with_email and user_with_email.id != user_id:
                return {'error': 'Email already registered'}, 400
        
        try:
            updated_user = facade.update_user(user_id, user_data)
            if updated_user:
                return {
                    'id': updated_user.id,
                    'first_name': updated_user.first_name,
                    'last_name': updated_user.last_name,
                    'email': updated_user.email
                }, 200
            else:
                return {'error': 'User not found'}, 404
        except ValueError as e:
            return {'error': str(e)}, 400
        except Exception as e:
            return {'error': f'Internal server error: {str(e)}'}, 500

    @jwt_required()
    def delete(self, user_id):
        """Delete a user (Admin only)"""
        current_user_id = get_jwt_identity()
        current_user = facade.get_user(current_user_id)

        if not current_user or not current_user.is_admin:
            return {'error': 'Admin privileges required'}, 403

        if facade.delete_user(user_id):
            return '', 204
        else:
            return {'error': 'User not found'}, 404
