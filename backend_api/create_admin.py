import sys
import os
from flask import Flask

# Add the project root to the Python path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

from app.services import facade
from app.models.user import User

def create_admin(first_name, last_name, email, password):
    """Creates an admin user."""
    app = Flask(__name__)
    app.config.from_object('config.DevelopmentConfig')

    with app.app_context():
        try:
            # Check if user already exists
            if facade.get_user_by_email(email):
                print(f"User with email {email} already exists.")
                return

            user_data = {
                'first_name': first_name,
                'last_name': last_name,
                'email': email,
                'password': password,
                'is_admin': True
            }
            admin = facade.create_user(user_data)
            print(f"Admin user {admin.email} created successfully with ID {admin.id}")
        except ValueError as e:
            print(f"Error: {e}")
        except Exception as e:
            print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python create_admin.py <first_name> <last_name> <email> <password>")
        sys.exit(1)
    
    first_name = sys.argv[1]
    last_name = sys.argv[2]
    email = sys.argv[3]
    password = sys.argv[4]
    
    create_admin(first_name, last_name, email, password)
