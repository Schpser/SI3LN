from app import db
from .base_model import BaseModel
from sqlalchemy.orm import validates

class Score(BaseModel):
    """Score model for game scores"""
    __tablename__ = 'scores'
    
    value = db.Column(db.Integer, nullable=False)
    level = db.Column(db.Integer, nullable=False)
    player_name = db.Column(db.String(100), nullable=False)
    
    # Foreign keys
    user_id = db.Column(db.String(36), db.ForeignKey('users.id'), nullable=False)
    game_id = db.Column(db.String(36), db.ForeignKey('games.id'), nullable=True)
    
    @validates('value')
    def validate_value(self, key, value):
        if value <= 0:
            raise ValueError("Score value must be greater than 0")
        return value
    
    @validates('level')
    def validate_level(self, key, level):
        if level < 1 or level > 5:
            raise ValueError("Level must be between 1 and 5")
        return level
    
    def to_dict(self):
        data = super().to_dict()
        data.update({
            'player_name': self.player_name,
            'value': self.value,
            'level': self.level,
            'user_id': self.user_id,
            'game_id': self.game_id
        })
        return data
