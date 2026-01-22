#include "../include/GameManager.hpp"
#include <iostream>
#include <ctime>

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
    : currentState(GameState::MAIN_MENU), previousState(GameState::MAIN_MENU),
      currentWorld(nullptr), currentLevel(nullptr), player(nullptr),
      currentUser("guest"), isGuest(true), highScore(0),
      sessionStartTime(0), screenWidth(Constants::DEFAULT_SCREEN_WIDTH),
      screenHeight(Constants::DEFAULT_SCREEN_HEIGHT),
      lastUpdateTime(0), deltaTime(0.0f) {
}

GameManager* GameManager::getInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

void GameManager::initialize(int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    
    sessionStartTime = getCurrentTime();
    lastUpdateTime = sessionStartTime;
    
    loadWorlds();
    
    std::cout << "GameManager initialized. Screen: " << screenWidth << "x" << screenHeight << std::endl;
}

void GameManager::loadWorlds() {
    // Create Space World
    WorldTheme spaceTheme;
    spaceTheme.name = "Space";
    spaceTheme.backgroundTexture = "worlds/space_bg.jpg";
    spaceTheme.playerBulletColor = "#00FFFF";
    spaceTheme.enemyBulletColor = "#FF0000";
    spaceTheme.enemyTextures = {"enemy_space_1", "enemy_space_2", "enemy_space_3"};
    spaceTheme.bossTexture = "boss_space";
    spaceTheme.musicTrack = "music/space.mp3";
    spaceTheme.difficultyMultiplier = 1.0f;
    
    createWorld("Space", spaceTheme);
    
    // Create Desert World
    WorldTheme desertTheme;
    desertTheme.name = "Desert";
    desertTheme.backgroundTexture = "worlds/desert_bg.jpg";
    desertTheme.playerBulletColor = "#FFD700";
    desertTheme.enemyBulletColor = "#8B4513";
    desertTheme.enemyTextures = {"enemy_desert_1", "enemy_desert_2", "enemy_desert_3"};
    desertTheme.bossTexture = "boss_desert";
    desertTheme.musicTrack = "music/desert.mp3";
    desertTheme.difficultyMultiplier = 1.2f;
    
    createWorld("Desert", desertTheme);
    
    // Create Forest World
    WorldTheme forestTheme;
    forestTheme.name = "Forest";
    forestTheme.backgroundTexture = "worlds/forest_bg.jpg";
    forestTheme.playerBulletColor = "#00FF00";
    forestTheme.enemyBulletColor = "#006400";
    forestTheme.enemyTextures = {"enemy_forest_1", "enemy_forest_2", "enemy_forest_3"};
    forestTheme.bossTexture = "boss_forest";
    forestTheme.musicTrack = "music/forest.mp3";
    forestTheme.difficultyMultiplier = 1.3f;
    
    createWorld("Forest", forestTheme);
    
    // Create Marine World
    WorldTheme marineTheme;
    marineTheme.name = "Marine";
    marineTheme.backgroundTexture = "worlds/marine_bg.jpg";
    marineTheme.playerBulletColor = "#1E90FF";
    marineTheme.enemyBulletColor = "#000080";
    marineTheme.enemyTextures = {"enemy_marine_1", "enemy_marine_2", "enemy_marine_3"};
    marineTheme.bossTexture = "boss_marine";
    marineTheme.musicTrack = "music/marine.mp3";
    marineTheme.difficultyMultiplier = 1.4f;
    
    createWorld("Marine", marineTheme);
    
    // Create Apocalyptic World
    WorldTheme apocalypticTheme;
    apocalypticTheme.name = "Apocalyptic";
    apocalypticTheme.backgroundTexture = "worlds/apocalyptic_bg.jpg";
    apocalypticTheme.playerBulletColor = "#FF4500";
    apocalypticTheme.enemyBulletColor = "#8B0000";
    apocalypticTheme.enemyTextures = {"enemy_apocalyptic_1", "enemy_apocalyptic_2", "enemy_apocalyptic_3"};
    apocalypticTheme.bossTexture = "boss_apocalyptic";
    apocalypticTheme.musicTrack = "music/apocalyptic.mp3";
    apocalypticTheme.difficultyMultiplier = 1.5f;
    
    createWorld("Apocalyptic", apocalypticTheme);
    
    std::cout << "Loaded " << worlds.size() << " worlds" << std::endl;
}

void GameManager::createWorld(const std::string& name, const WorldTheme& theme) {
    auto world = std::make_shared<World>(name, theme);
    worlds[name] = world;
}

void GameManager::update() {
    unsigned int currentTime = getCurrentTime();
    deltaTime = (currentTime - lastUpdateTime) / 1000.0f; // Convert to seconds
    lastUpdateTime = currentTime;
    
    switch (currentState) {
        case GameState::GAMEPLAY:
            if (currentLevel && player) {
                currentLevel->update(deltaTime, *player);
                player->update(deltaTime);
                
                // Check if level is completed
                if (currentLevel->isCompleted()) {
                    levelWin();
                }
                
                // Check if player is dead
                if (player->getLives() <= 0) {
                    gameOver();
                }
            }
            break;
            
        case GameState::PAUSE:
            // No updates during pause
            break;
            
        default:
            // Other states don't need game logic updates
            break;
    }
}

void GameManager::render() {
    switch (currentState) {
        case GameState::GAMEPLAY:
            if (currentLevel && player) {
                currentLevel->render();
                player->render();
            }
            break;
            
        default:
            // UI rendering will be handled by the HTML/CSS/JS frontend
            break;
    }
}

void GameManager::setState(GameState newState) {
    previousState = currentState;
    currentState = newState;
    std::cout << "State changed to: " << static_cast<int>(newState) << std::endl;
}

void GameManager::selectWorld(const std::string& worldName) {
    if (worlds.find(worldName) != worlds.end()) {
        currentWorld = worlds[worldName];
        std::cout << "Selected world: " << worldName << std::endl;
    } else {
        std::cerr << "World not found: " << worldName << std::endl;
    }
}

void GameManager::startLevel(int levelNumber) {
    if (!currentWorld) {
        std::cerr << "No world selected!" << std::endl;
        return;
    }
    
    if (!currentWorld->isLevelUnlocked(levelNumber)) {
        std::cerr << "Level " << levelNumber << " is locked!" << std::endl;
        return;
    }
    
    currentWorld->setCurrentLevel(levelNumber);
    currentLevel = std::make_shared<Level>(levelNumber, currentWorld.get());
    currentLevel->initialize();
    currentLevel->start();
    
    setState(GameState::GAMEPLAY);
    std::cout << "Started level " << levelNumber << std::endl;
}

void GameManager::restartLevel() {
    if (currentLevel) {
        currentLevel->reset();
        currentLevel->start();
        
        if (player) {
            player->resetScore();
        }
        
        setState(GameState::GAMEPLAY);
        std::cout << "Level restarted" << std::endl;
    }
}

void GameManager::nextLevel() {
    if (!currentWorld) return;
    
    int nextLevelNum = currentWorld->getCurrentLevel() + 1;
    
    if (nextLevelNum <= currentWorld->getTotalLevels()) {
        currentWorld->unlockNextLevel();
        startLevel(nextLevelNum);
    } else {
        std::cout << "All levels completed in this world!" << std::endl;
        setState(GameState::LEVEL_SELECT);
    }
}

bool GameManager::hasNextLevel() const {
    if (!currentWorld) return false;
    return currentWorld->getCurrentLevel() < currentWorld->getTotalLevels();
}

void GameManager::createPlayer(int characterId) {
    player = std::make_shared<Player>(
        screenWidth / 2.0f,
        screenHeight - 100.0f,
        characterId
    );
    std::cout << "Player created with character " << characterId << std::endl;
}

void GameManager::setUser(const std::string& username, bool guest) {
    currentUser = username;
    isGuest = guest;
    std::cout << "User set: " << username << (guest ? " (Guest)" : "") << std::endl;
}

void GameManager::updateHighScore() {
    if (player && player->getScore() > highScore) {
        highScore = player->getScore();
    }
}

void GameManager::saveScore() {
    if (!isGuest && player) {
        updateHighScore();
        // TODO: Save to file or database
        std::cout << "Score saved: " << player->getScore() << " for user " << currentUser << std::endl;
    }
}

void GameManager::setScreenSize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    std::cout << "Screen size updated: " << width << "x" << height << std::endl;
}

unsigned int GameManager::getCurrentTime() const {
    return static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
}

void GameManager::gameOver() {
    std::cout << "Game Over! Final score: " << (player ? player->getScore() : 0) << std::endl;
    saveScore();
    setState(GameState::GAME_OVER);
}

void GameManager::levelWin() {
    std::cout << "Level " << currentLevel->getLevelNumber() << " completed!" << std::endl;
    
    if (player) {
        // Bonus points for completing level
        player->addScore(currentLevel->getLevelNumber() * Constants::SCORE_LEVEL_MULTIPLIER);
    }
    
    saveScore();
    setState(GameState::LEVEL_WIN);
}

void GameManager::pauseGame() {
    if (currentState == GameState::GAMEPLAY) {
        setState(GameState::PAUSE);
    }
}

void GameManager::resumeGame() {
    if (currentState == GameState::PAUSE) {
        setState(GameState::GAMEPLAY);
    }
}

void GameManager::quitToMenu() {
    saveScore();
    currentLevel = nullptr;
    setState(GameState::MAIN_MENU);
}

void GameManager::cleanup() {
    worlds.clear();
    currentWorld = nullptr;
    currentLevel = nullptr;
    player = nullptr;
}

void GameManager::reset() {
    cleanup();
    currentState = GameState::MAIN_MENU;
    currentUser = "guest";
    isGuest = true;
    highScore = 0;
}
