# SI3LN Backend API - Quick Reference

## 🎮 Game Integration Checklist

### ✅ Enhanced Features Implemented

- [x] **User Profiles** - Character selection, playtime tracking
- [x] **Level Progression** - Track completed levels per world
- [x] **World Unlocking** - Automatic unlock system
- [x] **Best Scores** - Per-level best score and time tracking
- [x] **Leaderboards** - Global and per-level rankings
- [x] **Statistics** - Comprehensive player stats
- [x] **Database Schema** - Complete with all game tables

---

## 🚀 Quick Start

### 1. Start the API Server
```bash
cd /home/schpser/SI3LN/backend_api
python3 run.py
```

### 2. Access API Documentation
Open browser: `http://localhost:5000/api/docs`

### 3. Test Endpoints
Use the Swagger UI or the included `api_integration_example.py`

---

## 📋 Key Endpoints for Your Game

### Authentication
- `POST /api/v1/auth/register` - Create account
- `POST /api/v1/auth/login` - Login (get token)

### Profile
- `GET /api/v1/profile/me` - Get user profile
- `PUT /api/v1/profile/me` - Update profile
- `GET /api/v1/profile/me/stats` - Get statistics
- `POST /api/v1/profile/me/unlock-world` - Unlock world

### Level Management
- `GET /api/v1/levels/available` - Get unlocked levels
- `POST /api/v1/levels/complete` - Record completion
- `GET /api/v1/levels/my-completions` - Get user's completions
- `GET /api/v1/levels/leaderboard/{world}/{level}` - Get leaderboard

### Game Sessions
- `POST /api/v1/games` - Start game
- `PUT /api/v1/games/{id}` - Update game
- `POST /api/v1/games/{id}/complete` - Complete game

### Scores
- `GET /api/v1/scores?world={world}&level={level}` - Get leaderboard
- `POST /api/v1/scores` - Submit score

---

## 🎯 Game Flow Integration

### On Game Start
```python
# 1. User logs in
api.login(email, password)

# 2. Load profile
profile = api.get_profile()
selected_character = profile['selected_character']

# 3. Get available levels
levels = api.get_available_levels()
unlocked_worlds = levels['unlocked_worlds']
```

### During Level Selection
```python
# Show only unlocked worlds and levels
available = api.get_available_levels()

for world in available['unlocked_worlds']:
    levels = available['world_levels'][world]['available_levels']
    # Display in UI
```

### On Level Complete
```python
# Record the completion (auto-unlocks next level/world)
result = api.complete_level(
    world="Space",
    level=5,
    score=5500,
    time_elapsed=120.5
)

if result['next_world_unlocked']:
    show_message(f"New world unlocked: {result['next_world_unlocked']}")
```

### Save Character Selection
```python
api.update_character(character_index)
```

---

## 🗄️ Database Models

### UserProfile
- Character selection (0-7)
- Highest level reached
- Total playtime
- Unlocked worlds (comma-separated)

### LevelCompletion
- User + World + Level (unique)
- Best score
- Best time
- Completion timestamp

### Game (Session)
- Player name
- Current level/world
- Score
- Status (active/completed/game_over)
- Time elapsed

### Score
- Player name
- Score value
- Level
- Related game session

---

## 🔄 Progression Logic

### Level Unlocking
- Start: Space world, level 1 only
- Completing level N unlocks level N+1
- Max 10 levels per world

### World Unlocking
1. **Space** (default unlocked)
2. **Ocean** (unlock by completing Space level 10)
3. **Desert** (unlock by completing Ocean level 10)
4. **Forest** (unlock by completing Desert level 10)
5. **City** (unlock by completing Forest level 10)

### Best Score Tracking
- Each level completion compares with previous best
- Only updates if new score > old score
- Tracks both best score AND best time

---

## 📊 Statistics Available

### Profile Stats
- Total levels completed
- Total score across all games
- Best score achieved
- Total playtime
- Progress per world
- Recent scores

### Per-World Stats
- Levels completed
- Best score in world
- Total time spent

### Leaderboards
- Global top scores
- Per-level rankings
- Filterable by world and level

---

## 🔧 Files Changed/Created

### New Models
- `/app/models/user_profile.py` - User preferences
- `/app/models/level_completion.py` - Level tracking

### New API Routes
- `/app/api/v1/profile.py` - Profile management
- `/app/api/v1/levels.py` - Level completion

### Updated Files
- `/app/data/schema.sql` - Complete database schema
- `/app/models/__init__.py` - Export new models
- `/app/services/facade.py` - New business logic
- `/app/__init__.py` - Register new endpoints
- `/app/api/v1/scores.py` - World filtering
- `/app/api/v1/games.py` - Enhanced completion

### Documentation
- `GAME_API_GUIDE.md` - Complete API guide
- `api_integration_example.py` - Python examples
- `QUICK_REFERENCE.md` - This file

---

## 🐛 Testing

### Run Server
```bash
cd /home/schpser/SI3LN/backend_api
python3 run.py
```

### Test with curl
```bash
# Register user
curl -X POST http://localhost:5000/api/v1/auth/register \
  -H "Content-Type: application/json" \
  -d '{"first_name":"Test","last_name":"User","email":"test@test.com","password":"test123"}'

# Login
curl -X POST http://localhost:5000/api/v1/auth/login \
  -H "Content-Type: application/json" \
  -d '{"email":"test@test.com","password":"test123"}'
```

### Test with Python
```bash
cd /home/schpser/SI3LN/backend_api
python3 api_integration_example.py
```

---

## 📝 Next Steps for Your Game

1. **Add API calls to your game:**
   - Login screen → `POST /auth/login`
   - Level selector → `GET /levels/available`
   - Level complete → `POST /levels/complete`
   - Profile screen → `GET /profile/me`

2. **Store API token:**
   - Save token after login
   - Include in all subsequent requests
   - Handle token expiration

3. **Sync character selection:**
   - Load from API on start
   - Update API when changed

4. **Show leaderboards:**
   - Fetch from `/levels/leaderboard/{world}/{level}`
   - Display in game UI

5. **Track playtime:**
   - Update periodically: `PUT /profile/me`
   - Include total_playtime in requests

---

## 🎉 Summary

Your backend API is now **fully enhanced** to support all your game requirements:

✅ User profiles with character selection  
✅ Level progression and unlocking  
✅ World unlocking system  
✅ Best score/time tracking per level  
✅ Global and per-level leaderboards  
✅ Comprehensive player statistics  
✅ Complete database schema  
✅ RESTful API with Swagger docs  

**Ready for integration with your Python game!** 🚀
