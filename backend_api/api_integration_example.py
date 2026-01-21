"""
SI3LN Game - API Integration Example
This module demonstrates how to integrate the Python game with the backend API
"""

import requests
import json
from typing import Optional, Dict, List


class GameAPIClient:
    """Client for interacting with SI3LN Game Backend API"""
    
    def __init__(self, base_url: str = "http://localhost:5000/api/v1"):
        self.base_url = base_url
        self.token: Optional[str] = None
        self.user_id: Optional[str] = None
        
    def _headers(self) -> Dict[str, str]:
        """Get headers with authentication token"""
        headers = {"Content-Type": "application/json"}
        if self.token:
            headers["Authorization"] = f"Bearer {self.token}"
        return headers
    
    # ==================== AUTHENTICATION ====================
    
    def register(self, first_name: str, last_name: str, email: str, password: str) -> Dict:
        """Register a new user"""
        response = requests.post(
            f"{self.base_url}/auth/register",
            json={
                "first_name": first_name,
                "last_name": last_name,
                "email": email,
                "password": password
            }
        )
        return response.json()
    
    def login(self, email: str, password: str) -> bool:
        """Login and store authentication token"""
        response = requests.post(
            f"{self.base_url}/auth/login",
            json={"email": email, "password": password}
        )
        
        if response.status_code == 200:
            data = response.json()
            self.token = data.get("access_token")
            return True
        return False
    
    # ==================== PROFILE ====================
    
    def get_profile(self) -> Dict:
        """Get current user's profile"""
        response = requests.get(
            f"{self.base_url}/profile/me",
            headers=self._headers()
        )
        return response.json()
    
    def update_character(self, character_id: int) -> Dict:
        """Update selected character (0-7)"""
        response = requests.put(
            f"{self.base_url}/profile/me",
            headers=self._headers(),
            json={"selected_character": character_id}
        )
        return response.json()
    
    def update_playtime(self, total_seconds: float) -> Dict:
        """Update total playtime"""
        response = requests.put(
            f"{self.base_url}/profile/me",
            headers=self._headers(),
            json={"total_playtime": total_seconds}
        )
        return response.json()
    
    def get_stats(self) -> Dict:
        """Get detailed user statistics"""
        response = requests.get(
            f"{self.base_url}/profile/me/stats",
            headers=self._headers()
        )
        return response.json()
    
    # ==================== LEVELS ====================
    
    def get_available_levels(self) -> Dict:
        """Get available/unlocked levels and worlds"""
        response = requests.get(
            f"{self.base_url}/levels/available",
            headers=self._headers()
        )
        return response.json()
    
    def complete_level(self, world: str, level: int, score: int, time_elapsed: float) -> Dict:
        """Record level completion"""
        response = requests.post(
            f"{self.base_url}/levels/complete",
            headers=self._headers(),
            json={
                "world": world,
                "level": level,
                "score": score,
                "time_elapsed": time_elapsed
            }
        )
        return response.json()
    
    def get_my_completions(self, world: Optional[str] = None) -> List[Dict]:
        """Get user's level completions"""
        url = f"{self.base_url}/levels/my-completions"
        if world:
            url += f"?world={world}"
        
        response = requests.get(url, headers=self._headers())
        return response.json()
    
    def get_level_leaderboard(self, world: str, level: int, limit: int = 10) -> Dict:
        """Get leaderboard for a specific level"""
        response = requests.get(
            f"{self.base_url}/levels/leaderboard/{world}/{level}?limit={limit}",
            headers=self._headers()
        )
        return response.json()
    
    # ==================== GAMES ====================
    
    def start_game(self, player_name: str, level: int = 1, world: str = "Space") -> Dict:
        """Start a new game session"""
        response = requests.post(
            f"{self.base_url}/games",
            headers=self._headers(),
            json={
                "player_name": player_name,
                "level": level,
                "world": world
            }
        )
        return response.json()
    
    def update_game(self, game_id: str, score: int, status: str = "active", 
                   time_elapsed: float = 0.0, level: int = None) -> Dict:
        """Update game progress"""
        data = {"score": score, "status": status, "time_elapsed": time_elapsed}
        if level:
            data["level"] = level
        
        response = requests.put(
            f"{self.base_url}/games/{game_id}",
            headers=self._headers(),
            json=data
        )
        return response.json()
    
    def complete_game(self, game_id: str) -> Dict:
        """Mark game as completed (creates score and completion records)"""
        response = requests.post(
            f"{self.base_url}/games/{game_id}/complete",
            headers=self._headers()
        )
        return response.json()
    
    # ==================== SCORES ====================
    
    def get_leaderboard(self, world: Optional[str] = None, level: Optional[int] = None, 
                       limit: int = 10) -> List[Dict]:
        """Get global leaderboard"""
        params = [f"limit={limit}"]
        if world:
            params.append(f"world={world}")
        if level:
            params.append(f"level={level}")
        
        url = f"{self.base_url}/scores?{'&'.join(params)}"
        response = requests.get(url, headers=self._headers())
        return response.json()


# ==================== USAGE EXAMPLES ====================

def example_new_user_flow():
    """Example: New user registration and first game"""
    api = GameAPIClient()
    
    # Register new user
    print("Registering new user...")
    api.register(
        first_name="John",
        last_name="Doe",
        email="john.doe@example.com",
        password="SecurePassword123"
    )
    
    # Login
    print("Logging in...")
    if api.login("john.doe@example.com", "SecurePassword123"):
        print("✅ Login successful!")
    
    # Get profile
    profile = api.get_profile()
    print(f"Profile: Character {profile['selected_character']}, Unlocked: {profile['unlocked_worlds']}")
    
    # Get available levels
    available = api.get_available_levels()
    print(f"Available worlds: {available['unlocked_worlds']}")
    print(f"Highest level reached: {available['highest_level_reached']}")
    
    # Start a game
    game = api.start_game(player_name="John Doe", level=1, world="Space")
    game_id = game['id']
    print(f"Game started: {game_id}")
    
    # Simulate gameplay - update progress
    api.update_game(game_id, score=1500, time_elapsed=45.5)
    
    # Complete the level
    completion = api.complete_level(
        world="Space",
        level=1,
        score=1500,
        time_elapsed=45.5
    )
    print(f"Level completed! Next level unlocked: {completion['next_level_unlocked']}")
    
    # Complete the game
    final = api.complete_game(game_id)
    print(f"Game completed! Score recorded: {final['score']['value']}")


def example_returning_user():
    """Example: Returning user checks stats and continues"""
    api = GameAPIClient()
    
    # Login
    api.login("john.doe@example.com", "SecurePassword123")
    
    # Get stats
    stats = api.get_stats()
    print(f"Total levels completed: {stats['total_levels_completed']}")
    print(f"Best score: {stats['best_score']}")
    print(f"Total score: {stats['total_score']}")
    
    # Check world progress
    for world, progress in stats['worlds_progress'].items():
        print(f"{world}: {progress['levels_completed']} levels, Best: {progress['best_score']}")
    
    # Get available levels
    available = api.get_available_levels()
    
    # Continue with next available level
    for world in available['unlocked_worlds']:
        world_data = available['world_levels'][world]
        if world_data['available_levels']:
            next_level = max(world_data['available_levels'])
            print(f"Continue: {world} - Level {next_level}")
            break


def example_leaderboard_check():
    """Example: Check leaderboards"""
    api = GameAPIClient()
    api.login("john.doe@example.com", "SecurePassword123")
    
    # Global leaderboard
    print("\n=== Global Leaderboard ===")
    global_board = api.get_leaderboard(limit=5)
    for i, score in enumerate(global_board, 1):
        print(f"{i}. {score['player_name']}: {score['value']} (Level {score['level']})")
    
    # Level-specific leaderboard
    print("\n=== Space Level 5 Leaderboard ===")
    level_board = api.get_level_leaderboard("Space", 5, limit=5)
    for entry in level_board['leaderboard']:
        print(f"{entry['rank']}. {entry['player_name']}: {entry['score']} ({entry['time']}s)")


def example_character_change():
    """Example: Change selected character"""
    api = GameAPIClient()
    api.login("john.doe@example.com", "SecurePassword123")
    
    # Get current character
    profile = api.get_profile()
    print(f"Current character: {profile['selected_character']}")
    
    # Change character
    new_character = 3  # Character index 0-7
    updated = api.update_character(new_character)
    print(f"Updated to character: {updated['selected_character']}")


if __name__ == "__main__":
    print("SI3LN Game API Integration Examples")
    print("=" * 50)
    
    # Uncomment to run examples:
    # example_new_user_flow()
    # example_returning_user()
    # example_leaderboard_check()
    # example_character_change()
    
    print("\nReady to integrate with your game!")
