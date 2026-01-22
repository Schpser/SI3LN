# SI3LN - Space Invaders III Last Night
## C++ Game Engine

A high-performance C++ game engine for the SI3LN space shooter game, designed to handle core game logic while the UI is managed by HTML/CSS/JavaScript.

## 🎮 Features

### Core Game Mechanics
- **Multiple Worlds**: 5 unique worlds (Space, Desert, Forest, Marine, Apocalyptic)
- **Dynamic Levels**: 5 levels per world with progressive difficulty
- **Entity System**: Players, Enemies, Bosses, Bullets, Bonuses, Special Attacks
- **AI Behavior**: Multiple enemy movement patterns (horizontal, vertical, zigzag, circular, etc.)
- **Bonus System**: Shield, Mega Shot, Extra Life, Freeze Enemies, Speed Boost
- **Special Attacks**: Laser Beam, Bomb Blast, Wave Attack, Missile Barrage
- **Collision Detection**: Precise AABB collision system
- **Score System**: Points based on enemy types and level completion

### Technical Features
- **Modern C++17**: Using smart pointers, STL containers, and best practices
- **Entity Component System**: Flexible and extensible game object architecture
- **Game Manager**: Singleton pattern for centralized game state management
- **Wave System**: Progressive enemy spawning with cooldowns
- **Debuff System**: Player debuffs (freeze, blind, root)
- **Buff System**: Temporary power-ups with duration tracking

## 📁 Project Structure

```
game_engine_c++/
├── include/                    # Header files
│   ├── Constants.hpp          # Game constants and enums
│   ├── Vector2D.hpp           # 2D vector math
│   ├── Entity.hpp             # Base entity class
│   ├── Player.hpp             # Player entity
│   ├── Enemy.hpp              # Enemy entity
│   ├── Bullet.hpp             # Bullet entity
│   ├── Bonus.hpp              # Bonus pickups
│   ├── Explosion.hpp          # Visual effects
│   ├── SpecialAttack.hpp      # Special attacks
│   ├── World.hpp              # World management
│   ├── Level.hpp              # Level management
│   ├── GameManager.hpp        # Main game controller
│   └── Utils.hpp              # Utility functions
│
├── src/                       # Implementation files
│   ├── entities/              # Entity implementations
│   │   ├── Entity.cpp
│   │   ├── Player.cpp
│   │   ├── Enemy.cpp
│   │   ├── Bullet.cpp
│   │   ├── Bonus.cpp
│   │   ├── Explosion.cpp
│   │   └── SpecialAttack.cpp
│   │
│   ├── game/                  # Game logic
│   │   ├── World.cpp
│   │   ├── Level.cpp
│   │   └── GameManager.cpp
│   │
│   └── utils/                 # Utilities
│       └── Utils.cpp
│
├── build/                     # Build artifacts (generated)
├── bin/                       # Compiled executables (generated)
├── main.cpp                   # Entry point
├── Makefile                   # Build configuration
└── README.md                  # This file
```

## 🚀 Getting Started

### Prerequisites
- **C++ Compiler**: GCC 7+ or Clang 5+ with C++17 support
- **Make**: Build automation tool
- **Linux/Unix**: Developed and tested on Linux

### Building the Project

```bash
# Build the game engine
make

# Build with debug symbols
make debug

# Build optimized release version
make release

# Clean build files
make clean

# Rebuild from scratch
make rebuild
```

### Running the Game

```bash
# Run the game engine
make run

# Or run directly
./bin/game_engine

# With custom resolution
./bin/game_engine --width 1920 --height 1080
```

## 🎯 API Functions for Frontend Integration

The game engine provides a C API for integration with HTML/CSS/JavaScript frontends:

### Initialization
```c
void initGame(int width, int height);
void createPlayer(int characterId);
void selectWorld(const char* worldName);
void startLevel(int levelNumber);
```

### Player Controls
```c
void playerMoveLeft();
void playerMoveRight();
void playerMoveUp();
void playerMoveDown();
void playerStopMovement();
void playerShoot();
```

### Game State
```c
void pauseGame();
void resumeGame();
void quitToMenu();
void nextLevel();
void restartLevel();
```

### Game Loop
```c
void updateGame();      // Update game logic
void renderGame();      // Render entities
```

### Getters
```c
int getPlayerLives();
int getPlayerScore();
int getCurrentState();
bool isLevelCompleted();
```

## 🎮 Game Logic

### Player
- **Movement**: WASD or Arrow keys
- **Shooting**: Space bar with cooldown
- **Lives**: 5 lives by default
- **Bonuses**: Shield, Mega Shot, Speed Boost
- **Debuffs**: Freeze, Blind, Root

### Enemies
- **Types**: Basic, Fast, Tank, Shooter, Boss
- **AI**: Multiple movement patterns
- **Shooting**: Random shooting with cooldowns
- **Health**: Scales with level difficulty
- **Bonus Drops**: 15% chance to drop power-ups

### Levels
- **Wave System**: 3 waves per level
- **Progressive Difficulty**: Enemy count and health increase
- **Boss Fights**: Every 5th level
- **Completion**: Defeat all enemies

### Worlds
- **Space**: Basic difficulty (1.0x)
- **Desert**: Increased difficulty (1.2x)
- **Forest**: Higher difficulty (1.3x)
- **Marine**: Challenging difficulty (1.4x)
- **Apocalyptic**: Extreme difficulty (1.5x)

## 🔧 Configuration

Game constants can be modified in `include/Constants.hpp`:

```cpp
// Screen settings
constexpr int DEFAULT_SCREEN_WIDTH = 1024;
constexpr int DEFAULT_SCREEN_HEIGHT = 768;
constexpr int FPS = 60;

// Player settings
constexpr int MAX_LIVES = 5;
constexpr int PLAYER_SPEED = 5;

// Enemy settings
constexpr int ENEMY_SPEED = 2;
constexpr int BOSS_HEALTH = 10;

// Bonus settings
constexpr int SHIELD_DURATION = 3000;
constexpr int MEGA_SHOT_DURATION = 5000;
```

## 🧪 Testing

The game engine includes a standalone mode for testing:

```bash
# Run with default test scenario
./bin/game_engine

# The engine will:
# 1. Create a player with character 0
# 2. Select the Space world
# 3. Start level 1
# 4. Run game loop with FPS display
```

## 📊 Performance

- **Target FPS**: 60 FPS
- **Update Rate**: ~16.67ms per frame
- **Memory**: Smart pointers for automatic memory management
- **Optimization**: Release build with -O3 optimization

## 🔌 Integration with HTML/CSS/JS

The engine is designed to work with a web-based frontend:

1. **Compile as shared library**: Modify Makefile to create `.so` or `.dll`
2. **WebAssembly**: Use Emscripten to compile to WASM
3. **Native Integration**: Use Electron or similar framework
4. **API Communication**: JSON-based message passing

Example integration:
```javascript
// Initialize game
Module._initGame(1024, 768);

// Create player
Module._createPlayer(0);

// Select world and start level
Module._selectWorld("Space");
Module._startLevel(1);

// Game loop
function gameLoop() {
    Module._updateGame();
    Module._renderGame();
    
    // Get game state
    const lives = Module._getPlayerLives();
    const score = Module._getPlayerScore();
    
    requestAnimationFrame(gameLoop);
}
```

## 🎨 Assets (Managed by Frontend)

The C++ engine handles logic only. Assets are referenced by ID:
- **Player textures**: `player_0` to `player_7`
- **Enemy textures**: `enemy_basic`, `enemy_fast`, etc.
- **World backgrounds**: `worlds/space_bg.jpg`, etc.
- **Bullets**: `bullet_player`, `bullet_enemy`
- **Bonuses**: `bonus_shield`, `bonus_mega_shot`, etc.

## 🐛 Debugging

Build with debug symbols:
```bash
make debug
gdb ./bin/game_engine
```

Common issues:
- **Segfault**: Check for null pointers in entity access
- **Performance**: Profile with `gprof` or `valgrind`
- **Memory leaks**: Use `valgrind --leak-check=full`

## 📝 License

This project is developed for the SI3LN game. All rights reserved.

## 👥 Contributors

- Game Engine: C++ Implementation
- Original Game: Python version by schpser

## 🔮 Future Enhancements

- [ ] Particle system for visual effects
- [ ] Sound effect triggers
- [ ] Network multiplayer support
- [ ] Save/load game state
- [ ] Achievement system
- [ ] Replay system
- [ ] Level editor integration
- [ ] Physics engine integration

## 📞 Contact

For questions or issues, please open an issue in the project repository.

---

**Happy Gaming! 🎮🚀**
