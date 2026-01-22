# SI3LN Game Engine - Assets Directory Structure

This directory should contain game assets (textures, sounds, etc.)
The C++ engine references these assets by ID, and the actual loading
is handled by the frontend (HTML/CSS/JavaScript).

## Directory Structure

```
assets/
├── players/                   # Player character sprites
│   ├── player_0.png
│   ├── player_1.png
│   ├── player_2.png
│   ├── player_3.png
│   ├── player_4.png
│   ├── player_5.png
│   ├── player_6.png
│   └── player_7.png
│
├── enemies/                   # Enemy sprites
│   ├── space/
│   │   ├── enemy_basic.png
│   │   ├── enemy_fast.png
│   │   ├── enemy_tank.png
│   │   ├── enemy_shooter.png
│   │   └── boss.png
│   │
│   ├── desert/
│   ├── forest/
│   ├── marine/
│   └── apocalyptic/
│
├── bullets/                   # Bullet sprites
│   ├── player_bullet.png
│   └── enemy_bullet.png
│
├── bonuses/                   # Bonus item sprites
│   ├── bonus_shield.png
│   ├── bonus_mega_shot.png
│   ├── bonus_extra_life.png
│   ├── bonus_freeze.png
│   └── bonus_speed.png
│
├── effects/                   # Visual effects
│   ├── explosion.png
│   ├── special_laser.png
│   ├── special_bomb.png
│   ├── special_wave.png
│   └── special_missile.png
│
├── worlds/                    # World backgrounds
│   ├── space_bg.jpg
│   ├── desert_bg.jpg
│   ├── forest_bg.jpg
│   ├── marine_bg.jpg
│   └── apocalyptic_bg.jpg
│
├── ui/                        # UI elements
│   ├── button.png
│   ├── panel.png
│   ├── health_bar.png
│   └── score_panel.png
│
└── sounds/                    # Sound effects
    ├── shoot.wav
    ├── explosion.wav
    ├── bonus_pickup.wav
    ├── level_complete.wav
    └── game_over.wav
```

## Asset Requirements

### Players
- Size: 90x90 pixels
- Format: PNG with transparency
- 8 different characters

### Enemies
- Basic: 60x60 pixels
- Tank: 72x72 pixels
- Boss: 100x100 pixels
- Format: PNG with transparency

### Bullets
- Player: 15x25 pixels
- Enemy: 10x20 pixels
- Format: PNG with transparency

### Bonuses
- Size: 40x40 pixels
- Format: PNG with transparency

### Backgrounds
- Size: 1024x768 pixels (minimum)
- Format: JPG or PNG
- Should support scaling

## Asset Loading

The C++ engine does NOT load assets directly. Instead:

1. Engine references assets by ID (e.g., "player_0")
2. Frontend loads actual images/sounds
3. Frontend maps IDs to visual representations
4. Engine provides position and state data
5. Frontend renders based on engine data

Example mapping in JavaScript:
```javascript
const assetMap = {
    'player_0': 'assets/players/player_0.png',
    'enemy_basic': 'assets/enemies/space/enemy_basic.png',
    'bullet_player': 'assets/bullets/player_bullet.png',
    // ... etc
};
```

## Notes

- Copy assets from Python version: `/home/schpser/SI3LN_Python/assets/`
- Maintain consistent naming for easy reference
- Use sprite sheets for better performance
- Consider asset compression for web deployment
