from ninja import Router, Schema
from ninja.responses import Response
from django.contrib.auth import authenticate
from django.contrib.auth.models import User
from .jwt_auth import JWTAuth

router = Router()


class LoginSchema(Schema):
    username: str
    password: str


class RegisterSchema(Schema):
    username: str
    password: str
    email: str = ""


class TokenSchema(Schema):
    token: str
    username: str
    player_id: int


@router.post("/register", response=TokenSchema, tags=["Auth"])
def register(request, payload: RegisterSchema):
    """Register a new user"""
    from .models import Player
    
    # Check if user exists
    if User.objects.filter(username=payload.username).exists():
        return Response({"error": "Username already exists"}, status=400)
    
    # Create user
    user = User.objects.create_user(
        username=payload.username,
        password=payload.password,
        email=payload.email
    )
    
    # Create player profile
    player = Player.objects.create(
        user=user,
        username=payload.username
    )
    
    # Generate JWT token (expires in 24 hours)
    token = JWTAuth.create_token(user)
    
    return {
        "token": token,
        "username": user.username,
        "player_id": player.id
    }


@router.post("/login", response=TokenSchema, tags=["Auth"])
def login(request, payload: LoginSchema):
    """Login user and get JWT token (valid for 24 hours)"""
    from .models import Player
    
    user = authenticate(username=payload.username, password=payload.password)
    
    if user is None:
        return Response({"error": "Invalid credentials"}, status=401)
    
    # Get or create player profile
    player, _ = Player.objects.get_or_create(
        user=user,
        defaults={'username': user.username, 'email': user.email or ''}
    )
    
    # Generate JWT token (expires in 24 hours)
    token = JWTAuth.create_token(user)
    
    return {
        "token": token,
        "username": user.username,
        "player_id": player.id
    }


@router.post("/logout", tags=["Auth"])
def logout(request):
    """Logout user (client should discard token)"""
    # JWT is stateless, so we just return success
    # Client should discard the token
    return {"message": "Logged out successfully. Please discard your token."}


@router.post("/refresh", response=TokenSchema, tags=["Auth"])
def refresh_token(request):
    """Refresh JWT token (extends expiration by 24 hours)"""
    from .models import Player
    
    auth_header = request.headers.get('Authorization', '')
    if not auth_header.startswith('Bearer '):
        return Response({"error": "No token provided"}, status=401)
    
    token = auth_header[7:]
    new_token = JWTAuth.refresh_token(token)
    
    if not new_token:
        return Response({"error": "Invalid or expired token"}, status=401)
    
    # Get user info
    user = JWTAuth.get_user_from_token(new_token)
    if not user:
        return Response({"error": "Invalid token"}, status=401)
    
    player = Player.objects.get(user=user)
    
    return {
        "token": new_token,
        "username": user.username,
        "player_id": player.id
    }


@router.get("/me", tags=["Auth"])
def get_current_user(request):
    """Get current authenticated user info"""
    auth_header = request.headers.get('Authorization', '')
    if not auth_header.startswith('Bearer '):
        return Response({"error": "Not authenticated"}, status=401)
    
    token = auth_header[7:]
    user = JWTAuth.get_user_from_token(token)
    
    if not user:
        return Response({"error": "Invalid or expired token. Please login again."}, status=401)
    
    from .models import Player
    player = Player.objects.get(user=user)
    
    return {
        "username": user.username,
        "email": user.email,
        "player_id": player.id,
        "total_score": player.total_score,
        "games_played": player.games_played
    }
