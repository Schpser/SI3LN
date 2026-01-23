#include "Game.h"
#include <iostream>
#include <algorithm>

namespace SI3LN {

Game::Game()
    : window(nullptr)
    , renderer(nullptr)
    , currentState(GameState::MAIN_MENU)
    , running(false)
    , fullscreen(false)
    , screenWidth(DEFAULT_SCREEN_WIDTH)
    , screenHeight(DEFAULT_SCREEN_HEIGHT)
    , lastFrameTime(0)
    , currentScore(0)
    , currentLevel(1)
    , currentWorld("Space")
    , lives(MAX_LIVES)
    , playerTexture(nullptr)
    , enemyTexture(nullptr)
    , backgroundTexture(nullptr)
    , font(nullptr)
    , fontLarge(nullptr)
{
}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow(
        "S I 3 L N - Space Invaders III Last Night",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Load assets
    loadAssets();
    
    std::cout << "Game initialized successfully!" << std::endl;
    return true;
}

void Game::loadAssets() {
    std::cout << "Loading assets..." << std::endl;
    
    // Load fonts
    font = TTF_OpenFont("assets/fonts/arial.ttf", FONT_SIZE_MEDIUM);
    fontLarge = TTF_OpenFont("assets/fonts/arial.ttf", FONT_SIZE_LARGE);
    
    if (!font || !fontLarge) {
        std::cerr << "Warning: Could not load fonts. Using system default." << std::endl;
    }
    
    // Load textures (placeholder paths - update with actual asset paths)
    playerTexture = loadTexture("assets/players/player_1/AnimateDiff_00001.001.png");
    enemyTexture = loadTexture("assets/enemies/Apocalyptic_world/boss_enemy.png");
    backgroundTexture = loadTexture("assets/worlds/background_apocalyptic.jpg");
    
    std::cout << "Assets loaded!" << std::endl;
}

SDL_Texture* Game::loadTexture(const std::string& path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        std::cerr << "Warning: Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
    }
    return texture;
}

void Game::cleanup() {
    // Destroy textures
    if (playerTexture) SDL_DestroyTexture(playerTexture);
    if (enemyTexture) SDL_DestroyTexture(enemyTexture);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    
    // Close fonts
    if (font) TTF_CloseFont(font);
    if (fontLarge) TTF_CloseFont(fontLarge);
    
    // Destroy renderer and window
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    
    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    std::cout << "Game cleaned up successfully!" << std::endl;
}

void Game::run() {
    if (!init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return;
    }
    
    running = true;
    lastFrameTime = SDL_GetTicks();
    
    // Main game loop
    while (running) {
        uint32_t currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentTime;
        
        handleEvents();
        update(deltaTime);
        render();
        
        // Cap frame rate
        SDL_Delay(FRAME_DELAY);
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
                
            case SDL_KEYDOWN:
                handleKeyPress(event.key.keysym.sym);
                break;
                
            case SDL_MOUSEBUTTONDOWN:
                handleMouseClick(event.button.x, event.button.y);
                break;
                
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    screenWidth = event.window.data1;
                    screenHeight = event.window.data2;
                }
                break;
        }
    }
}

void Game::handleKeyPress(SDL_Keycode key) {
    switch (key) {
        case SDLK_ESCAPE:
            if (currentState == GameState::GAMEPLAY) {
                changeState(GameState::PAUSE);
            } else {
                running = false;
            }
            break;
            
        case SDLK_F11:
            fullscreen = !fullscreen;
            if (fullscreen) {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            } else {
                SDL_SetWindowFullscreen(window, 0);
            }
            break;
            
        case SDLK_SPACE:
            if (currentState == GameState::GAMEPLAY && player && playerBullets.size() < MAX_PLAYER_BULLETS) {
                if (player->canShoot()) {
                    auto bullet = std::make_shared<Bullet>(
                        player->getPosition().x,
                        player->getPosition().y - player->getHeight() / 2,
                        true,
                        screenHeight,
                        Colors::CYAN
                    );
                    playerBullets.push_back(bullet);
                    player->resetShootCooldown();
                }
            }
            break;
            
        case SDLK_RETURN:
            if (currentState == GameState::MAIN_MENU) {
                changeState(GameState::GAMEPLAY);
                spawnEnemies();
                player = std::make_unique<Player>(
                    screenWidth / 2.0f,
                    screenHeight - 100.0f,
                    playerTexture,
                    screenWidth,
                    screenHeight
                );
            }
            break;
    }
}

void Game::handleMouseClick(int x, int y) {
    // Handle mouse clicks based on current state
    // TODO: Implement UI button handling
}

void Game::update(float deltaTime) {
    switch (currentState) {
        case GameState::GAMEPLAY:
            updateGameplay(deltaTime);
            break;
            
        // TODO: Add other state updates
        default:
            break;
    }
}

void Game::updateGameplay(float deltaTime) {
    // Update player
    if (player) {
        const uint8_t* keyState = SDL_GetKeyboardState(nullptr);
        player->handleInput(keyState);
        player->update(deltaTime);
    }
    
    // Update enemies
    for (auto& enemy : enemies) {
        enemy->update(deltaTime);
        
        // Enemy shooting
        if (enemy->canShoot()) {
            auto bullet = std::make_shared<Bullet>(
                enemy->getPosition().x,
                enemy->getPosition().y + enemy->getHeight() / 2,
                false,
                screenHeight,
                Colors::RED
            );
            enemyBullets.push_back(bullet);
        }
    }
    
    // Update bullets
    for (auto& bullet : playerBullets) {
        bullet->update(deltaTime);
    }
    for (auto& bullet : enemyBullets) {
        bullet->update(deltaTime);
    }
    
    // Remove dead bullets
    playerBullets.erase(
        std::remove_if(playerBullets.begin(), playerBullets.end(),
            [](const auto& b) { return !b->isAlive(); }),
        playerBullets.end()
    );
    enemyBullets.erase(
        std::remove_if(enemyBullets.begin(), enemyBullets.end(),
            [](const auto& b) { return !b->isAlive(); }),
        enemyBullets.end()
    );
    
    // Check collisions
    checkCollisions();
    
    // Check win condition
    if (enemies.empty()) {
        std::cout << "Level complete!" << std::endl;
        currentLevel++;
        spawnEnemies();
    }
    
    // Check game over
    if (lives <= 0) {
        changeState(GameState::GAME_OVER);
    }
}

void Game::checkCollisions() {
    // Player bullets vs enemies
    for (auto bulletIt = playerBullets.begin(); bulletIt != playerBullets.end();) {
        bool bulletHit = false;
        
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            if ((*bulletIt)->intersects(**enemyIt)) {
                currentScore += 10 * currentLevel;
                (*bulletIt)->kill();
                enemyIt = enemies.erase(enemyIt);
                bulletHit = true;
                break;
            } else {
                ++enemyIt;
            }
        }
        
        if (bulletHit || !(*bulletIt)->isAlive()) {
            bulletIt = playerBullets.erase(bulletIt);
        } else {
            ++bulletIt;
        }
    }
    
    // Enemy bullets vs player
    if (player) {
        for (auto bulletIt = enemyBullets.begin(); bulletIt != enemyBullets.end();) {
            if ((*bulletIt)->intersects(*player)) {
                lives--;
                bulletIt = enemyBullets.erase(bulletIt);
                std::cout << "Player hit! Lives: " << lives << std::endl;
            } else {
                ++bulletIt;
            }
        }
    }
}

void Game::spawnEnemies() {
    enemies.clear();
    
    int rows = std::min(ENEMY_SPAWN_BASE_ROWS + currentLevel / 2, ENEMY_SPAWN_MAX_ROWS);
    int cols = std::min(ENEMY_SPAWN_BASE_COLS + currentLevel / 2, ENEMY_SPAWN_MAX_COLS);
    
    int totalWidth = cols * (ENEMY_SIZE + ENEMY_SPACING_X);
    int startX = (screenWidth - totalWidth) / 2;
    int startY = 80;
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float x = startX + col * (ENEMY_SIZE + ENEMY_SPACING_X) + ENEMY_SIZE / 2;
            float y = startY + row * (ENEMY_SIZE + ENEMY_SPACING_Y) + ENEMY_SIZE / 2;
            
            auto enemy = std::make_shared<Enemy>(x, y, enemyTexture, screenWidth, currentLevel);
            enemies.push_back(enemy);
        }
    }
    
}

void Game::render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    switch (currentState) {
        case GameState::GAMEPLAY:
            renderGameplay();
            break;
            
        case GameState::MAIN_MENU:
            // Render main menu
            renderText("S I 3 L N", screenWidth / 2 - 100, 100, fontLarge, Colors::CYAN);
            renderText("Press ENTER to start", screenWidth / 2 - 150, 300, font, Colors::WHITE);
            renderText("Press ESC to quit", screenWidth / 2 - 120, 350, font, Colors::WHITE);
            break;
            
        case GameState::GAME_OVER:
            renderText("GAME OVER", screenWidth / 2 - 150, 200, fontLarge, Colors::RED);
            renderText("Score: " + std::to_string(currentScore), screenWidth / 2 - 100, 300, font, Colors::WHITE);
            renderText("Press ESC to quit", screenWidth / 2 - 120, 400, font, Colors::WHITE);
            break;
            
        default:
            break;
    }
    
    SDL_RenderPresent(renderer);
}

void Game::renderGameplay() {
    // Draw background
    if (backgroundTexture) {
        SDL_Rect bgRect = {0, 0, screenWidth, screenHeight};
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
    }
    
    // Render player
    if (player) {
        player->render(renderer);
    }
    
    // Render enemies
    for (auto& enemy : enemies) {
        enemy->render(renderer);
    }
    
    // Render bullets
    for (auto& bullet : playerBullets) {
        bullet->render(renderer);
    }
    for (auto& bullet : enemyBullets) {
        bullet->render(renderer);
    }
    
    // Render HUD
    renderText("Score: " + std::to_string(currentScore), 10, 10, font, Colors::WHITE);
    renderText("Lives: " + std::to_string(lives), 10, 50, font, Colors::WHITE);
    renderText("Level: " + std::to_string(currentLevel), 10, 90, font, Colors::WHITE);
}

void Game::renderText(const std::string& text, int x, int y, TTF_Font* font, const Color& color) {
    if (!font) return;
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color.toSDL());
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Game::changeState(GameState newState) {
    currentState = newState;
}

} // namespace SI3LN
