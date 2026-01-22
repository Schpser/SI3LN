from app import db
from .base_model import BaseModel
from sqlalchemy.orm import validates

class LevelCompletion(BaseModel):
    """Track level completion and best performances"""
    __tablename__ = 'level_completions'
    
    user_id = db.Column(db.String(36), db.ForeignKey('users.id'), nullable=False)
    world = db.Column(db.String(50), nullable=False)
    level = db.Column(db.Integer, nullable=False)
    best_score = db.Column(db.Integer, default=0)
    best_time = db.Column(db.Float, default=0.0)
    completed_at = db.Column(db.DateTime, nullable=False)

    user = db.relationship('User', backref=db.backref('level_completions', lazy=True, cascade='all, delete-orphan'))

    __table_args__ = (db.UniqueConstraint('user_id', 'world', 'level', name='_user_world_level_uc'),)
    
    @validates('level')
    def validate_level(self, key, level):
        if level < 1 or level > 5:
            raise ValueError("Level must be between 1 and 5")
        return level
    
    @validates('world')
    def validate_world(self, key, world):
        valid_worlds = ["Space", "Ocean", "Desert", "Forest", "SpaceShip"]
        if world not in valid_worlds:
            raise ValueError(f"World must be one of {valid_worlds}")
        return world
    
    @validates('best_score')
    def validate_score(self, key, score):
        if score < 0:
            raise ValueError("Score cannot be negative")
        return score
    
    def to_dict(self):
        data = super().to_dict()
        data.update({
            'user_id': self.user_id,
            'world': self.world,
            'level': self.level,
            'best_score': self.best_score,
            'best_time': self.best_time,
            'completed_at': self.completed_at.isoformat() if self.completed_at else None
        })
        return data
