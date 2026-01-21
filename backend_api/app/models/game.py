from app import db
from .base_model import BaseModel
from sqlalchemy.orm import validates

class Game(BaseModel):
    """Game session model"""
    __tablename__ = 'games'
    
    player_name = db.Column(db.String(100), nullable=False)
    level = db.Column(db.Integer, default=1, nullable=False)
    world = db.Column(db.String(50), default="Space", nullable=False)
    score = db.Column(db.Integer, default=0)
    status = db.Column(db.String(20), default="active")  # active, completed, game_over
    time_elapsed = db.Column(db.Float, default=0.0)
    
    # Foreign key to user
    user_id = db.Column(db.String(36), db.ForeignKey('users.id'), nullable=False)
    
    @validates('level')
    def validate_level(self, key, level):
        if level < 1 or level > 10:
            raise ValueError("Level must be between 1 and 10")
        return level
    
    @validates('world')
    def validate_world(self, key, world):
        valid_worlds = ["Space", "Ocean", "Desert", "Forest", "City"]
        if world not in valid_worlds:
            raise ValueError(f"World must be one of {valid_worlds}")
        return world
    
    def to_dict(self):
        data = super().to_dict()
        data.update({
            'player_name': self.player_name,
            'level': self.level,
            'world': self.world,
            'score': self.score,
            'status': self.status,
            'time_elapsed': self.time_elapsed,
            'user_id': self.user_id
        })
        return data
