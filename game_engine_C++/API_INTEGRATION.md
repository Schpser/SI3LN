# Integration Guide: C++ Game ↔ Django Ninja API

## How It Works

Your C++ game and Django Ninja API are **separate processes** that communicate via HTTP:

```
C++ Game Engine  ──HTTP Requests──>  Django Ninja API  ──>  SQLite Database
(Port varies)                        (Port 8000)
```

## Setup Steps

### 1. Install Dependencies (C++)

```bash
sudo apt-get install libcurl4-openssl-dev nlohmann-json3-dev
```

### 2. Update CMakeLists.txt

Add to your CMakeLists.txt:

```cmake
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(YourGameTarget 
    CURL::libcurl
    nlohmann_json::nlohmann_json
)
```

### 3. Use APIClient in Your Game

```cpp
#include "APIClient.h"

// In your Game class initialization:
SI3LN::APIClient apiClient;

// When game starts:
int playerId = apiClient.createPlayer("PlayerName", "player@email.com");
int sessionId = apiClient.startGameSession(playerId);

// During gameplay (update score periodically):
apiClient.updateGameSession(sessionId, currentScore, currentLevel, enemiesKilled, durationSeconds);

// When game ends:
apiClient.endGameSession(sessionId, finalScore, finalLevel, gameCompleted);
```

## Integration Points

### Game Start
```cpp
void Game::start() {
    sessionId_ = apiClient_.startGameSession(playerId_, worldId_);
    // ... start game loop
}
```

### Score Update (every 10 seconds or on level change)
```cpp
void Game::updateScore() {
    if (time() - lastApiUpdate_ > 10) {
        apiClient_.updateGameSession(sessionId_, score_, level_, enemiesKilled_, 
                                     duration_);
        lastApiUpdate_ = time();
    }
}
```

### Game End
```cpp
void Game::end() {
    bool completed = (playerLives_ > 0);
    apiClient_.endGameSession(sessionId_, score_, level_, completed);
}
```

## Testing

1. **Start the API server:**
   ```bash
   cd /home/schpser/SI3LN/api
   python3 manage.py runserver
   ```

2. **Run your C++ game:**
   ```bash
   cd /home/schpser/SI3LN/game_engine_C++
   ./build/SI3LN
   ```

3. **Check the data:**
   - Visit: http://127.0.0.1:8000/api/docs
   - Or: http://127.0.0.1:8000/api/game/sessions

## Alternative: Python Wrapper

If you don't want to modify C++, you can wrap your game with Python:

```python
import subprocess
import requests

# Start game
game_process = subprocess.Popen(['./build/SI3LN'])

# Create session via API
response = requests.post('http://127.0.0.1:8000/api/game/sessions', 
                        json={'player_id': 1})

# Wait for game to finish
game_process.wait()

# Update final score
requests.patch(f'http://127.0.0.1:8000/api/game/sessions/{session_id}',
              json={'score': final_score, 'completed': True})
```

## What You Need to Do

Choose one option:

**Option A:** Add APIClient to C++ (recommended for real-time updates)
**Option B:** Use Python wrapper (simpler, less real-time)

Would you like me to help integrate one of these options?
