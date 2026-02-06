from ninja import Schema
from datetime import datetime
from typing import Optional


class PlayerSchema(Schema):
    id: int
    username: str
    email: str
    total_score: int
    games_played: int
    created_at: datetime


class PlayerCreateSchema(Schema):
    username: str
    email: str


class GameSessionSchema(Schema):
    id: int
    player_id: int
    world_id: Optional[int] = None
    score: int
    level_reached: int
    enemies_killed: int
    duration_seconds: int
    completed: bool
    started_at: datetime
    ended_at: Optional[datetime] = None


class GameSessionCreateSchema(Schema):
    player_id: int
    world_id: Optional[int] = None


class GameSessionUpdateSchema(Schema):
    score: Optional[int] = None
    level_reached: Optional[int] = None
    duration_seconds: Optional[int] = None
    ended_at: Optional[datetime] = None


class LeaderboardEntrySchema(Schema):
    rank: int
    player_id: int
    player_username: str
    score: int
    level_reached: int
    world_name: Optional[str] = None
    created_at: datetime


class MessageSchema(Schema):
    message: str
