from app.models.base_model import BaseModel
import re

class User(BaseModel):
    __tablename__ = 'users'

    from app import db
    
    first_name = db.Column(db.String(50), nullable=False)
    last_name = db.Column(db.String(50), nullable=False)
    email = db.Column(db.String(120), nullable=False, unique=True)
    password = db.Column(db.String(255), nullable=False)
    is_admin = db.Column(db.Boolean, default=False)

    games = db.relationship('Game', backref='owner', lazy=True, cascade='all, delete-orphan')
    scores = db.relationship('Score', backref='owner', lazy=True, cascade='all, delete-orphan')

    def __init__(self, first_name, last_name, email, password=None, is_admin=False):
        if len(first_name) == 0 or len(first_name) > 50:
            raise ValueError("first name must be between 1 and 50 characters")
        if len(last_name) == 0 or len(last_name) > 50:
            raise ValueError("last name must be between 1 and 50 characters")
        if not isinstance(is_admin, bool):
            raise ValueError("is_admin must be a boolean")
        if not email:
            raise ValueError("email is required")
        if not re.match(r"[^@]+@[^@]+\.[^@]+", email):
            raise ValueError("invalid email format")

        self.first_name = first_name
        self.last_name = last_name
        self.email = email
        self.is_admin = is_admin

        if password:
            self.hash_password(password)

    def hash_password(self, password):
        """Hash the password before storing it."""
        from app import bcrypt
        from flask import current_app
        pepper = current_app.config.get('PEPPER', '')
        peppered_password = password + pepper
        self.password = bcrypt.generate_password_hash(peppered_password).decode('utf-8')

    def verify_password(self, password):
        """Verify the hashed password."""
        from app import bcrypt
        from flask import current_app
        if not self.password:
            return False
        pepper = current_app.config.get('PEPPER', '')
        peppered_password = password + pepper
        return bcrypt.check_password_hash(self.password, peppered_password)

    def to_dict(self):
        return {
            'id': self.id,
            'first_name': self.first_name,
            'last_name': self.last_name,
            'email': self.email,
            'created_at': self.created_at.isoformat(),
            'updated_at': self.updated_at.isoformat()
        }
