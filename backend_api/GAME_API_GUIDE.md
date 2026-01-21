# SI3LN Game Backend API - Enhanced Features

## New Enhancements for Game Integration

### Overview
The backend API has been enhanced to fully support your Python game's requirements for:
- Level progression tracking
- World unlocking system
- User profiles with character selection
- Best scores per level/world
- Comprehensive statistics

---

## New Database Tables

### 1. **user_profiles**
Stores user preferences and progression data.

**Columns:**
- `user_id` - Foreign key to users table
- `selected_character` - Character selection (0-7)
- `highest_level_reached` - Highest level unlocked
- `total_playtime` - Total gameplay time in seconds
- `unlocked_worlds` - Comma-separated list of unlocked worlds

### 2. **level_completions**
Tracks best performance for each level/world combination.

**Columns:**
- `user_id` - Foreign key to users table
- `world` - World name (Space, Ocean, Desert, Forest, City)
- `level` - Level number (1-10)
- `best_score` - Best score achieved
- `best_time` - Fastest completion time
- `completed_at` - First completion timestamp

---

## New API Endpoints

### Profile Management

#### `GET /api/v1/profile/me`
Get current user's profile with stats.

**Response:**
```json
{
  "id": "uuid",
  "user_id": "uuid",
  "selected_character": 0,
  "highest_level_reached": 5,
  "total_playtime": 3600.5,
  "unlocked_worlds": ["Space", "Ocean"],
  "user": {
    "id": "uuid",
    "first_name": "John",
    "last_name": "Doe",
    "email": "john@example.com"
  }
}
```

#### `PUT /api/v1/profile/me`
Update user profile (character selection, playtime).

**Request Body:**
```json
{
  "selected_character": 2,
  "total_playtime": 4000.0
}
```

#### `POST /api/v1/profile/me/unlock-world`
Unlock a new world for the user.

**Request Body:**
```json
{
  "world": "Ocean"
}
```

**Response:**
```json
{
  "message": "World Ocean unlocked",
  "unlocked_worlds": ["Space", "Ocean"]
}
```

#### `GET /api/v1/profile/me/stats`
Get detailed user statistics.

**Response:**
```json
{
  "profile": {...},
  "total_levels_completed": 15,
  "total_score": 25000,
  "best_score": 5500,
  "worlds_progress": {
    "Space": {
      "levels_completed": 10,
      "best_score": 5500,
      "total_time": 1200.5
    }
  },
  "recent_scores": [...]
}
```

---

### Level Completion Tracking

#### `POST /api/v1/levels/complete`
Record a level completion (auto-unlocks next level/world).

**Request Body:**
```json
{
  "world": "Space",
  "level": 5,
  "score": 5500,
  "time_elapsed": 120.5
}
```

**Response:**
```json
{
  "completion": {
    "id": "uuid",
    "world": "Space",
    "level": 5,
    "best_score": 5500,
    "best_time": 120.5
  },
  "next_level_unlocked": true,
  "next_world_unlocked": null,
  "is_new_best": true
}
```

**Logic:**
- Completing level 10 in a world unlocks the next world
- Each completion updates best score/time if better
- Automatically advances `highest_level_reached`

#### `GET /api/v1/levels/my-completions`
Get all completed levels for current user.

**Query Parameters:**
- `world` (optional) - Filter by world

**Response:**
```json
[
  {
    "id": "uuid",
    "world": "Space",
    "level": 1,
    "best_score": 1200,
    "best_time": 45.3,
    "completed_at": "2026-01-21T10:00:00"
  },
  ...
]
```

#### `GET /api/v1/levels/my-completions/{world}/{level}`
Get specific level completion details.

**Example:** `GET /api/v1/levels/my-completions/Space/5`

#### `GET /api/v1/levels/available`
Get available/unlocked levels and worlds for current user.

**Response:**
```json
{
  "unlocked_worlds": ["Space", "Ocean"],
  "world_levels": {
    "Space": {
      "unlocked": true,
      "completed_levels": [1, 2, 3, 4, 5],
      "available_levels": [1, 2, 3, 4, 5, 6],
      "total_levels": 10
    },
    "Ocean": {
      "unlocked": true,
      "completed_levels": [],
      "available_levels": [1],
      "total_levels": 10
    }
  },
  "highest_level_reached": 6
}
```

#### `GET /api/v1/levels/leaderboard/{world}/{level}`
Get leaderboard for a specific world/level.

**Example:** `GET /api/v1/levels/leaderboard/Space/5?limit=10`

**Response:**
```json
{
  "world": "Space",
  "level": 5,
  "leaderboard": [
    {
      "rank": 1,
      "player_name": "John Doe",
      "score": 5500,
      "time": 120.5,
      "completed_at": "2026-01-21T10:00:00"
    },
    ...
  ]
}
```

---

### Enhanced Existing Endpoints

#### `GET /api/v1/scores?world={world}&level={level}&limit={limit}`
Now supports filtering by world.

**Example:** `GET /api/v1/scores?world=Space&level=5&limit=10`

#### `POST /api/v1/games/{game_id}/complete`
Enhanced to automatically create score and level completion records.

**Response:**
```json
{
  "game": {...},
  "score": {...},
  "completion": {...}
}
```

#### `GET /api/v1/games/by-world/{world}`
Get all games for current user in a specific world.

**Example:** `GET /api/v1/games/by-world/Space`

---

## Game Integration Guide

### 1. **On User Login**
```python
# After successful authentication
response = requests.get(
    "http://api/v1/profile/me",
    headers={"Authorization": f"Bearer {token}"}
)
profile = response.json()

# Load user preferences
selected_character = profile['selected_character']
unlocked_worlds = profile['unlocked_worlds']
```

### 2. **Level Selection Screen**
```python
# Get available levels
response = requests.get(
    "http://api/v1/levels/available",
    headers={"Authorization": f"Bearer {token}"}
)
data = response.json()

# Show only unlocked worlds and levels
for world in data['unlocked_worlds']:
    levels = data['world_levels'][world]['available_levels']
    # Display levels in UI
```

### 3. **On Level Complete**
```python
# Record completion
response = requests.post(
    "http://api/v1/levels/complete",
    headers={"Authorization": f"Bearer {token}"},
    json={
        "world": current_world,
        "level": current_level,
        "score": final_score,
        "time_elapsed": time_taken
    }
)

result = response.json()
if result['next_world_unlocked']:
    show_message(f"New world unlocked: {result['next_world_unlocked']}")
```

### 4. **Save Character Selection**
```python
# When user selects character
requests.put(
    "http://api/v1/profile/me",
    headers={"Authorization": f"Bearer {token}"},
    json={"selected_character": character_index}
)
```

### 5. **View Leaderboard**
```python
# Get top scores for current level
response = requests.get(
    f"http://api/v1/levels/leaderboard/{world}/{level}?limit=10",
    headers={"Authorization": f"Bearer {token}"}
)
leaderboard = response.json()['leaderboard']
```

---

## World Progression Logic

The API implements automatic progression:

1. **Starting:** Only "Space" world unlocked, only level 1 available
2. **Level Progression:** Completing level N unlocks level N+1 (max 10)
3. **World Unlocking:** Completing level 10 of a world unlocks the next world

**World Order:**
1. Space → 2. Ocean → 3. Desert → 4. Forest → 5. City

---

## Testing the API

### Start the server:
```bash
cd /home/schpser/SI3LN/backend_api
python3 run.py
```

### Access Swagger Documentation:
```
http://localhost:5000/api/docs
```

### Example workflow:
1. Register/Login user
2. Check profile: `GET /api/v1/profile/me`
3. Get available levels: `GET /api/v1/levels/available`
4. Start a game: `POST /api/v1/games`
5. Complete level: `POST /api/v1/levels/complete`
6. View stats: `GET /api/v1/profile/me/stats`

---

## Migration from Old Schema

If you have an existing database, run:

```bash
cd /home/schpser/SI3LN/backend_api
rm -f instance/development.db  # Remove old database
python3 run.py  # Create new database with all tables
```

---

## Summary of Enhancements

✅ **Database Schema** - Complete schema with all game tables  
✅ **User Profiles** - Character selection, playtime tracking  
✅ **Level Completions** - Best scores and times per level/world  
✅ **World Unlocking** - Automatic progression system  
✅ **Leaderboards** - Per-level and per-world rankings  
✅ **Statistics** - Comprehensive player stats  
✅ **API Endpoints** - Full CRUD for all game features  

Your backend is now fully ready to support your Python game! 🎮
