#pragma once

#include "Constants.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <memory>
#include <string>

namespace SI3LN {

class Game {
public:
    Game();
    ~Game();
    
    // Main game loop
    void run();
    
private:
    // Initialization
    bool init();
    void loadAssets();
    void cleanup();
    
    // Game loop functions
    void handleEvents();
    void update(float deltaTime);
    void render();
    
    // Gameplay functions
    void updateGameplay(float deltaTime);
    void renderGameplay();
    void spawnEnemies();
    void checkCollisions();
    
    // Input handling
    void handleKeyPress(SDL_Keycode key);
    void handleMouseClick(int x, int y);
    
    // State management
    void changeState(GameState newState);
    
    // SDL components
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    // Game state
    GameState currentState;
    bool running;
    bool fullscreen;
    
    // Screen dimensions
    int screenWidth;
    int screenHeight;
    
    // Timing
    uint32_t lastFrameTime;
    
    // Game data
    int currentScore;
    int currentLevel;
    std::string currentWorld;
    int lives;
    
    // Entities
    std::unique_ptr<Player> player;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Bullet>> playerBullets;
    std::vector<std::shared_ptr<Bullet>> enemyBullets;
    
    // Assets
    SDL_Texture* playerTexture;
    SDL_Texture* enemyTexture;
    SDL_Texture* backgroundTexture;
    TTF_Font* font;
    TTF_Font* fontLarge;
    
    // Helper functions
    SDL_Texture* loadTexture(const std::string& path);
    void renderText(const std::string& text, int x, int y, TTF_Font* font, const Color& color);
};

} // namespace SI3LN