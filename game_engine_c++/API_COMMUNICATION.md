# API Communication Guide - C++ Engine ↔ Frontend

## 🔌 Overview

The C++ game engine **does NOT handle PNG/JPG files** directly. Instead:

1. **C++ Engine** handles:
   - Game logic (physics, collisions, AI)
   - Entity states (positions, health, scores)
   - Level progression
   - Asset ID management

2. **Frontend (HTML/CSS/JS)** handles:
   - Loading PNG/JPG images
   - Rendering graphics
   - UI/Menus
   - User input
   - Audio playback

## 📡 Communication Flow

```
┌─────────────────┐          JSON/API          ┌──────────────────┐
│                 │ ◄────────────────────────► │                  │
│  C++ Engine     │                            │  JavaScript      │
│  (Game Logic)   │   • Entity positions       │  Frontend        │
│                 │   • Asset IDs              │  (Rendering)     │
│                 │   • Game state             │                  │
└─────────────────┘                            └──────────────────┘
        ▲                                               │
        │                                               │
        │           Entity Data + Asset IDs             │
        │                                               │
        └───────────────────────────────────────────────┘
                    Frontend loads & renders
```

## 🎮 Using the API

### Initialization

```javascript
// Initialize the game engine
API_InitGame(1024, 768);

// Initialize asset registry
APIBridge.getInstance()->initialize();

// Create player
API_CreatePlayer(0); // Character ID 0-7

// Select world
API_SelectWorld("Space");

// Start level
API_StartLevel(1);
```

### Game Loop

```javascript
function gameLoop() {
    // Update game logic (C++)
    API_Update(deltaTime);
    
    // Get render data from C++
    const renderJSON = API_GetRenderDataJSON();
    const renderData = JSON.parse(renderJSON);
    
    // Render entities using the data
    renderEntities(renderData);
    
    // Get game state
    const stateJSON = API_GetGameStateJSON();
    const gameState = JSON.parse(stateJSON);
    
    // Update UI
    updateUI(gameState);
    
    // Continue loop
    requestAnimationFrame(gameLoop);
}
```

### Player Controls

```javascript
// Keyboard input
document.addEventListener('keydown', (e) => {
    switch(e.key) {
        case 'ArrowLeft':
        case 'a':
            API_PlayerMoveLeft(true);
            break;
        case 'ArrowRight':
        case 'd':
            API_PlayerMoveRight(true);
            break;
        case 'ArrowUp':
        case 'w':
            API_PlayerMoveUp(true);
            break;
        case 'ArrowDown':
        case 's':
            API_PlayerMoveDown(true);
            break;
        case ' ':
            API_PlayerShoot();
            break;
    }
});

document.addEventListener('keyup', (e) => {
    switch(e.key) {
        case 'ArrowLeft':
        case 'a':
        case 'ArrowRight':
        case 'd':
            API_PlayerMoveLeft(false);
            API_PlayerMoveRight(false);
            break;
        // ... etc
    }
});
```

## 📊 Data Structures

### Render Data JSON

```json
{
  "entities": [
    {
      "id": "player",
      "type": "player",
      "asset": "assets/players/1000055338.png",
      "x": 512,
      "y": 668,
      "width": 90,
      "height": 90,
      "rotation": 0,
      "scale": 1.0,
      "visible": true,
      "layer": 5
    },
    {
      "id": "enemy_0",
      "type": "enemy",
      "asset": "assets/enemies/Space_world/enemy.png",
      "x": 100,
      "y": 150,
      "width": 60,
      "height": 60,
      "rotation": 0,
      "scale": 1.0,
      "visible": true,
      "layer": 4
    }
  ],
  "background": "assets/worlds/background_space.jpg"
}
```

### Game State JSON

```json
{
  "state": "2",
  "lives": 5,
  "score": 1500,
  "level": 1,
  "world": "Space",
  "enemiesRemaining": 12,
  "paused": false,
  "gameOver": false,
  "levelComplete": false
}
```

### All Assets JSON

```json
{
  "assets": [
    "assets/players/1000055338.png",
    "assets/players/1000055339.png",
    "assets/worlds/background_space.jpg",
    "assets/enemies/Space_world/enemy.png",
    "assets/sprites/player/pb_space.png",
    ...
  ]
}
```

## 🎨 Rendering in JavaScript

### Canvas2D Example

```javascript
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');
const loadedImages = {};

// Preload all assets
async function preloadAssets() {
    const assetsJSON = API_GetAllAssetsJSON();
    const assets = JSON.parse(assetsJSON);
    
    for (const assetPath of assets.assets) {
        await loadImage(assetPath);
    }
}

function loadImage(path) {
    return new Promise((resolve, reject) => {
        const img = new Image();
        img.onload = () => {
            loadedImages[path] = img;
            resolve(img);
        };
        img.onerror = reject;
        img.src = path;
    });
}

function renderEntities(renderData) {
    // Clear canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // Draw background
    const bgImg = loadedImages[renderData.background];
    if (bgImg) {
        ctx.drawImage(bgImg, 0, 0, canvas.width, canvas.height);
    }
    
    // Sort entities by layer
    const entities = renderData.entities.sort((a, b) => a.layer - b.layer);
    
    // Draw each entity
    for (const entity of entities) {
        if (!entity.visible) continue;
        
        const img = loadedImages[entity.asset];
        if (!img) continue;
        
        ctx.save();
        
        // Apply transformations
        ctx.translate(entity.x, entity.y);
        if (entity.rotation) {
            ctx.rotate(entity.rotation * Math.PI / 180);
        }
        if (entity.scale !== 1.0) {
            ctx.scale(entity.scale, entity.scale);
        }
        
        // Draw centered
        ctx.drawImage(
            img,
            -entity.width / 2,
            -entity.height / 2,
            entity.width,
            entity.height
        );
        
        ctx.restore();
    }
}

function updateUI(gameState) {
    document.getElementById('lives').textContent = gameState.lives;
    document.getElementById('score').textContent = gameState.score;
    document.getElementById('level').textContent = gameState.level;
    document.getElementById('world').textContent = gameState.world;
    
    if (gameState.gameOver) {
        showGameOverScreen();
    } else if (gameState.levelComplete) {
        showLevelCompleteScreen();
    }
}
```

## 🔧 Integration Methods

### Option 1: WebAssembly (Emscripten)

```bash
# Compile to WASM
emcc main.cpp src/**/*.cpp -Iinclude \
  -o game.js \
  -s WASM=1 \
  -s EXPORTED_FUNCTIONS='["_API_InitGame","_API_CreatePlayer","_API_Update",...]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
  -s ALLOW_MEMORY_GROWTH=1
```

```html
<script src="game.js"></script>
<script>
Module.onRuntimeInitialized = function() {
    // Call C functions
    Module._API_InitGame(1024, 768);
    
    // Or use ccall/cwrap
    const initGame = Module.cwrap('API_InitGame', null, ['number', 'number']);
    initGame(1024, 768);
};
</script>
```

### Option 2: Native with Node.js Addon

```bash
# Create Node.js addon
node-gyp configure
node-gyp build
```

```javascript
const gameEngine = require('./build/Release/game_engine');

gameEngine.initGame(1024, 768);
gameEngine.createPlayer(0);
// etc...
```

### Option 3: REST API Server

```cpp
// Create HTTP server that exposes C++ functions
// Use libraries like cpp-httplib or crow

CROW_ROUTE(app, "/api/init")
([](const crow::request& req){
    // Parse JSON
    // Call C++ functions
    // Return JSON response
});
```

## 📝 Available API Functions

### Initialization
- `API_InitGame(width, height)` - Initialize game
- `API_CreatePlayer(characterId)` - Create player (0-7)
- `API_SelectWorld(worldName)` - Select world
- `API_StartLevel(levelNumber)` - Start level

### Game Control
- `API_PauseGame()` - Pause game
- `API_ResumeGame()` - Resume game
- `API_RestartLevel()` - Restart current level
- `API_NextLevel()` - Go to next level
- `API_QuitToMenu()` - Return to menu

### Player Controls
- `API_PlayerMoveLeft(pressed)` - Move left
- `API_PlayerMoveRight(pressed)` - Move right
- `API_PlayerMoveUp(pressed)` - Move up
- `API_PlayerMoveDown(pressed)` - Move down
- `API_PlayerShoot()` - Shoot
- `API_PlayerSpecialAttack()` - Special attack

### Game Loop
- `API_Update(deltaTime)` - Update game logic

### Data Queries (return JSON strings)
- `API_GetRenderDataJSON()` - Get all entities to render
- `API_GetGameStateJSON()` - Get game state
- `API_GetAllAssetsJSON()` - Get all asset paths

### Simple Queries
- `API_GetPlayerLives()` - Get lives (int)
- `API_GetPlayerScore()` - Get score (int)
- `API_GetCurrentLevel()` - Get level (int)
- `API_GetCurrentWorld()` - Get world (string)
- `API_IsGameOver()` - Check game over (bool)
- `API_IsLevelComplete()` - Check level complete (bool)
- `API_IsPaused()` - Check paused (bool)

### Asset Queries
- `API_GetPlayerAsset(characterId)` - Get player sprite path
- `API_GetWorldBackground(worldName)` - Get background path

## 🎯 Complete Example

See `examples/frontend_integration.html` for a complete working example.

## 📚 Additional Resources

- [README.md](README.md) - Project overview
- [QUICK_REFERENCE.md](QUICK_REFERENCE.md) - Class reference
- [examples/](examples/) - Integration examples
- [config.json](config.json) - Game configuration

## 🐛 Troubleshooting

**Q: Images not loading?**
- Check that asset paths are correct
- Ensure assets are preloaded before rendering
- Check browser console for 404 errors

**Q: Game running too fast/slow?**
- Pass correct deltaTime to `API_Update()`
- Use `requestAnimationFrame` for consistent timing

**Q: Can't call C++ functions?**
- Ensure WASM module is fully loaded
- Check function names match exported functions
- Verify parameter types match

**Q: Memory leaks?**
- C++ uses smart pointers for automatic cleanup
- Ensure `API_QuitToMenu()` is called when leaving game

---

**For questions or issues, see project documentation or open an issue.**
