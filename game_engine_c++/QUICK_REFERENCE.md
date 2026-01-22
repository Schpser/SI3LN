# SI3LN C++ Game Engine - Quick Reference Guide

## 📚 Class Overview

### Core Classes

#### **Entity** (Base class)
All game objects inherit from Entity
- Position, velocity, dimensions
- Collision detection
- Screen boundary checking
- Active/inactive state

#### **Player**
The player-controlled character
- Lives management (default: 5)
- Score tracking
- Character selection (8 options)
- Bonus states (shield, mega shot)
- Debuff handling (freeze, blind, root)
- Movement controls (WASD + arrows)
- Shooting with cooldown

#### **Enemy**
AI-controlled opponents
- 5 types: Basic, Fast, Tank, Shooter, Boss
- Health system (scales with level)
- Movement patterns: horizontal, vertical, circular, zigzag, etc.
- AI aggro system
- Shooting behavior
- Bonus dropping (15% chance)

#### **Bullet**
Projectiles from player or enemies
- Directional movement
- Damage values
- Player vs enemy differentiation
- Automatic cleanup when off-screen

#### **Bonus**
Power-up items
- 5 types: Shield, Mega Shot, Extra Life, Freeze, Speed Boost
- Falling animation
- Limited lifetime (10 seconds)
- Collision with player triggers effect

#### **Explosion**
Visual effects for hits
- Animated scaling
- Configurable duration (default: 500ms)
- Auto-cleanup when finished

#### **SpecialAttack**
Powerful player abilities
- 4 types: Laser Beam, Bomb Blast, Wave Attack, Missile Barrage
- Area-of-effect damage
- Range-based enemy detection
- Cooldown system (10 seconds)

### Management Classes

#### **World**
Represents a game world (Space, Desert, etc.)
- Theme configuration (backgrounds, colors)
- Level progression tracking
- Difficulty multipliers
- Level unlocking system

#### **Level**
Individual level instance
- Enemy spawning and waves
- Collision handling
- Entity lifecycle management
- Completion checking
- Score tracking

#### **GameManager** (Singleton)
Central game controller
- Game state management
- World/level selection
- Player lifecycle
- Main game loop
- High score tracking
- User management

## 🎮 Game Flow

```
1. Initialize GameManager
2. Load Worlds (5 worlds, 5 levels each)
3. Create Player (select character)
4. Select World
5. Start Level
   ├─ Initialize level
   ├─ Spawn wave 1
   ├─ Game loop:
   │  ├─ Update entities
   │  ├─ Handle collisions
   │  ├─ Check wave completion
   │  ├─ Spawn next wave
   │  └─ Check level completion
   └─ Level complete or game over
6. Next level or return to menu
```

## 🔧 Key Constants

```cpp
// Screen
DEFAULT_SCREEN_WIDTH = 1024
DEFAULT_SCREEN_HEIGHT = 768
FPS = 60

// Player
MAX_LIVES = 5
PLAYER_SPEED = 5
PLAYER_SHOOT_COOLDOWN = 250ms

// Enemy
ENEMY_SPEED = 2
BOSS_HEALTH = 10
ENEMY_SHOOT_COOLDOWN = 2000ms

// Scoring
SCORE_ENEMY = 100
SCORE_BOSS = 1000
SCORE_LEVEL_MULTIPLIER = 50

// Bonuses
SHIELD_DURATION = 3000ms
MEGA_SHOT_DURATION = 5000ms
FREEZE_DURATION = 2000ms
```

## 🎯 Enemy Types

| Type    | Health | Speed | Cooldown | Pattern    | Score |
|---------|--------|-------|----------|------------|-------|
| Basic   | 1      | 2     | 3000ms   | Horizontal | 100   |
| Fast    | 1      | 3     | 2000ms   | Zigzag     | 150   |
| Tank    | 3      | 1.4   | 4000ms   | Vertical   | 200   |
| Shooter | 2      | 2     | 1500ms   | Circular   | 180   |
| Boss    | 10     | 1     | 1000ms   | Diagonal   | 1000  |

## 💎 Bonus Types

| Bonus         | Effect                        | Duration |
|---------------|-------------------------------|----------|
| Shield        | Protect from 1 hit            | Until hit|
| Mega Shot     | Increased bullet damage       | 5000ms   |
| Extra Life    | +1 life                       | Instant  |
| Freeze        | Freeze all enemies            | 2000ms   |
| Speed Boost   | Faster movement               | 5000ms   |

## 🚀 Special Attacks

| Attack          | Damage | Range | Duration | Effect                    |
|-----------------|--------|-------|----------|---------------------------|
| Laser Beam      | 3      | 1000  | 1000ms   | Vertical line damage      |
| Bomb Blast      | 10     | 150   | 500ms    | Circular explosion        |
| Wave Attack     | 4      | 300   | 2000ms   | Expanding wave            |
| Missile Barrage | 6      | 250   | 1500ms   | Multiple projectiles      |

## 🌍 Worlds

| World        | Difficulty | Levels | Boss Level |
|--------------|------------|--------|------------|
| Space        | 1.0x       | 5      | Level 5    |
| Desert       | 1.2x       | 5      | Level 5    |
| Forest       | 1.3x       | 5      | Level 5    |
| Marine       | 1.4x       | 5      | Level 5    |
| Apocalyptic  | 1.5x       | 5      | Level 5    |

## 📊 Collision Matrix

|                | Player | Enemy | Player Bullet | Enemy Bullet | Bonus |
|----------------|--------|-------|---------------|--------------|-------|
| Player         | -      | ✓     | -             | ✓            | ✓     |
| Enemy          | ✓      | -     | ✓             | -            | -     |
| Player Bullet  | -      | ✓     | -             | -            | -     |
| Enemy Bullet   | ✓      | -     | -             | -            | -     |
| Bonus          | ✓      | -     | -             | -            | -     |

## 🔄 Game States

```cpp
enum class GameState {
    MAIN_MENU,        // Initial menu
    LOGIN,            // User authentication
    REGISTER,         // Account creation
    CHARACTER_SELECT, // Choose character
    LEVEL_SELECT,     // Choose world/level
    GAMEPLAY,         // Active gameplay
    PAUSE,            // Paused gameplay
    GAME_OVER,        // Player lost all lives
    LEVEL_WIN,        // Level completed
    PROFILE           // User profile screen
}
```

## 🎨 Movement Patterns

```cpp
HORIZONTAL  - Left/right oscillation
VERTICAL    - Up/down movement
DIAGONAL    - Combined X/Y movement
CIRCULAR    - Orbit around center point
ZIGZAG      - Horizontal with sine wave
RANDOM      - Random direction changes
```

## 💻 Build Commands

```bash
make              # Build project
make run          # Build and run
make clean        # Remove build files
make rebuild      # Clean + build
make debug        # Build with debug symbols
make release      # Optimized build
make help         # Show all commands
```

## 🐛 Debug Tips

1. **Check entity lifecycle**: Verify entities are properly created/destroyed
2. **Monitor collision**: Ensure bounding boxes are correct
3. **Track game state**: Print state transitions
4. **Validate pointers**: Check for null before dereferencing
5. **Profile performance**: Use timing utilities for bottlenecks

## 📝 Common Tasks

### Add a New Enemy Type
1. Add enum to `EnemyType` in Constants.hpp
2. Add case in Enemy.cpp constructor
3. Define properties (health, speed, pattern)
4. Create texture reference

### Add a New World
1. Create WorldTheme in GameManager::loadWorlds()
2. Add background asset
3. Define difficulty multiplier
4. Add to world selection UI

### Add a New Bonus
1. Add enum to `BonusType` in Constants.hpp
2. Add case in Bonus.cpp constructor
3. Implement effect in Level::applyBonus()
4. Create texture reference

### Modify Difficulty
Edit values in Constants.hpp:
- `DIFFICULTY_MULTIPLIER`
- `BASE_ENEMY_COUNT`
- World-specific multipliers

---

**For more details, see README.md**
