from ninja import Router
from typing import List
from django.shortcuts import get_object_or_404
from .models import Player, GameSession
from .schemas import (
    PlayerSchema,
    PlayerCreateSchema,
    GameSessionSchema,
    GameSessionCreateSchema,
    GameSessionUpdateSchema,
    LeaderboardEntrySchema,
    MessageSchema,
)
from .auth.auth_decorators import jwt_auth

router = Router()


# Player endpoints (protected)
@router.get("/players", response=List[PlayerSchema], tags=["Players"], auth=jwt_auth)
def list_players(request):
    """Get all players (requires authentication)"""
    return Player.objects.all()


@router.post("/players", response=PlayerSchema, tags=["Players"])
def create_player(request, payload: PlayerCreateSchema):
    """Create a new player (public for game registration)"""
    player = Player.objects.create(**payload.dict())
    return player


@router.get("/players/{player_id}", response=PlayerSchema, tags=["Players"], auth=jwt_auth)
def get_player(request, player_id: int):
    """Get a specific player by ID (requires authentication)"""
    return get_object_or_404(Player, id=player_id)


@router.put("/players/{player_id}", response=PlayerSchema, tags=["Players"], auth=jwt_auth)
def update_player(request, player_id: int, payload: PlayerCreateSchema):
    """Update a player (requires authentication)"""
    player = get_object_or_404(Player, id=player_id)
    for attr, value in payload.dict().items():
        setattr(player, attr, value)
    player.save()
    return player


@router.delete("/players/{player_id}", response=MessageSchema, tags=["Players"], auth=jwt_auth)
def delete_player(request, player_id: int):
    """Delete a player (requires authentication)"""
    player = get_object_or_404(Player, id=player_id)
    player.delete()
    return {"message": "Player deleted successfully"}


# Game Session endpoints (protected)
@router.get("/sessions", response=List[GameSessionSchema], tags=["Game Sessions"], auth=jwt_auth)
def list_sessions(request, player_id: int = None, world_id: int = None):
    """Get all game sessions (requires authentication)"""
    sessions = GameSession.objects.all()
    if player_id:
        sessions = sessions.filter(player_id=player_id)
    if world_id:
        sessions = sessions.filter(world_id=world_id)
    return sessions.order_by("-started_at")


@router.post("/sessions", response=GameSessionSchema, tags=["Game Sessions"], auth=jwt_auth)
def create_session(request, payload: GameSessionCreateSchema):
    """Create a new game session (requires authentication)"""
    session = GameSession.objects.create(**payload.dict())
    return session


@router.get("/sessions/{session_id}", response=GameSessionSchema, tags=["Game Sessions"], auth=jwt_auth)
def get_session(request, session_id: int):
    """Get a specific game session (requires authentication)"""
    return get_object_or_404(GameSession, id=session_id)


@router.patch("/sessions/{session_id}", response=GameSessionSchema, tags=["Game Sessions"], auth=jwt_auth)
def update_session(request, session_id: int, payload: GameSessionUpdateSchema):
    """Update a game session (requires authentication)"""
    session = get_object_or_404(GameSession, id=session_id)
    
    for attr, value in payload.dict(exclude_unset=True).items():
        setattr(session, attr, value)
    
    session.save()
    
    # Update player stats
    if payload.score is not None:
        player = session.player
        player.total_score += payload.score
        player.games_played += 1
        player.save()
    
    return session


@router.delete("/sessions/{session_id}", response=MessageSchema, tags=["Game Sessions"], auth=jwt_auth)
def delete_session(request, session_id: int):
    """Delete a game session (requires authentication)"""
    session = get_object_or_404(GameSession, id=session_id)
    session.delete()
    return {"message": "Session deleted successfully"}


# Leaderboard endpoints (public - no auth required for viewing)
@router.get("/leaderboard", response=List[LeaderboardEntrySchema], tags=["Leaderboard"])
def get_leaderboard(request, world_id: int = None, limit: int = 10):
    """Get the leaderboard (public access)"""
    sessions = GameSession.objects.select_related("player", "world")
    
    if world_id:
        sessions = sessions.filter(world_id=world_id)
    
    sessions = sessions.order_by("-score")[:limit]
    
    leaderboard = []
    for rank, session in enumerate(sessions, start=1):
        leaderboard.append({
            "rank": rank,
            "player_id": session.player.id,
            "player_username": session.player.username,
            "score": session.score,
            "level_reached": session.level_reached,
            "world_name": session.world.name if session.world else None,
            "created_at": session.started_at,
        })
    
    return leaderboard


# Stats endpoint (public - no auth required)
@router.get("/stats", tags=["Stats"])
def get_stats(request):
    """Get overall game statistics (public access)"""
    from django.db.models import Sum, Avg, Max
    
    total_players = Player.objects.count()
    total_sessions = GameSession.objects.count()
    total_score = Player.objects.aggregate(Sum("total_score"))["total_score__sum"] or 0
    avg_score = GameSession.objects.aggregate(Avg("score"))["score__avg"] or 0
    highest_score = GameSession.objects.aggregate(Max("score"))["score__max"] or 0
    
    return {
        "total_players": total_players,
        "total_sessions": total_sessions,
        "total_score": total_score,
        "average_score": round(avg_score, 2),
        "highest_score": highest_score,
    }
