from app import db
from .base_model import BaseModel
from sqlalchemy.orm import validates

class UserProfile(BaseModel):
    """User profile and preferences model"""
    __tablename__ = 'user_profiles'
    
    user_id = db.Column(db.String(36), db.ForeignKey('users.id'), nullable=False, unique=True)
    selected_character = db.Column(db.Integer, default=0)
    highest_level_reached = db.Column(db.Integer, default=1)
    total_playtime = db.Column(db.Float, default=0.0)
    unlocked_worlds = db.Column(db.String(255), default='Space')
    achievements = db.Column(db.String(255), default='Space')
    settings = db.Column(db.String(255), default='Space')
    last_play_date = db.Column(db.DateTime(10), default=db.func.now())

    user = db.relationship('User', backref=db.backref('profile', uselist=False, cascade='all, delete-orphan'))
    
    @validates('selected_character')
    def validate_character(self, key, character):
        if character < 1 or character > 8:
            raise ValueError("Character must be between 1 and 8")
        return character
    
    @validates('highest_level_reached')
    def validate_level(self, key, level):
        if level < 1 or level > 5:
            raise ValueError("Level must be between 1 and 5")
        return level
    
    def get_unlocked_worlds(self):
        """Get list of unlocked worlds"""
        if not self.unlocked_worlds:
            return ['Space']
        return [w.strip() for w in self.unlocked_worlds.split(',')]
    
    def unlock_world(self, world):
        """Add a world to unlocked worlds"""
        worlds = self.get_unlocked_worlds()
        if world not in worlds:
            worlds.append(world)
            self.unlocked_worlds = ','.join(worlds)
    
    def is_world_unlocked(self, world):
        """Check if a world is unlocked"""
        return world in self.get_unlocked_worlds()
    
    def to_dict(self):
        data = super().to_dict()
        data.update({
            'user_id': self.user_id,
            'selected_character': self.selected_character,
            'highest_level_reached': self.highest_level_reached,
            'total_playtime': self.total_playtime,
            'unlocked_worlds': self.get_unlocked_worlds()
        })
        return data
