#include "../include/APIBridge.hpp"
#include "../include/GameManager.hpp"
#include "../include/AssetRegistry.hpp"
#include <iostream>
#include <sstream>
#include <cstring>

APIBridge* APIBridge::instance = nullptr;

APIBridge::APIBridge() {
}

APIBridge* APIBridge::getInstance() {
    if (instance == nullptr) {
        instance = new APIBridge();
    }
    return instance;
}

void APIBridge::initialize() {
    std::cout << "Initializing API Bridge..." << std::endl;
    AssetRegistry::getInstance()->initialize();
}

void APIBridge::setLogCallback(std::function<void(const std::string&)> callback) {
    logCallback = callback;
}

void APIBridge::setEventCallback(std::function<void(const std::string&)> callback) {
    eventCallback = callback;
}

// ===== GAME CONTROL =====

void APIBridge::initGame(int screenWidth, int screenHeight) {
    GameManager::getInstance()->initialize(screenWidth, screenHeight);
    log("Game initialized: " + std::to_string(screenWidth) + "x" + std::to_string(screenHeight));
}

void APIBridge::createPlayer(int characterId) {
    GameManager::getInstance()->createPlayer(characterId);
    log("Player created with character " + std::to_string(characterId));
    sendEvent("player_created", std::to_string(characterId));
}

void APIBridge::selectWorld(const std::string& worldName) {
    GameManager::getInstance()->selectWorld(worldName);
    log("Selected world: " + worldName);
    sendEvent("world_selected", worldName);
}

void APIBridge::startLevel(int levelNumber) {
    GameManager::getInstance()->startLevel(levelNumber);
    log("Started level " + std::to_string(levelNumber));
    sendEvent("level_started", std::to_string(levelNumber));
}

void APIBridge::pauseGame() {
    GameManager::getInstance()->pauseGame();
    sendEvent("game_paused");
}

void APIBridge::resumeGame() {
    GameManager::getInstance()->resumeGame();
    sendEvent("game_resumed");
}

void APIBridge::restartLevel() {
    GameManager::getInstance()->restartLevel();
    sendEvent("level_restarted");
}

void APIBridge::nextLevel() {
    GameManager::getInstance()->nextLevel();
    sendEvent("next_level");
}

void APIBridge::quitToMenu() {
    GameManager::getInstance()->quitToMenu();
    sendEvent("quit_to_menu");
}

void APIBridge::exitGame() {
    GameManager::getInstance()->cleanup();
    log("Game exited");
}

// ===== PLAYER CONTROLS =====

void APIBridge::playerMoveLeft(bool pressed) {
    auto player = GameManager::getInstance()->getPlayer();
    if (player) {
        if (pressed) player->moveLeft();
        else player->stopMovement();
    }
}

void APIBridge::playerMoveRight(bool pressed) {
    auto player = GameManager::getInstance()->getPlayer();
    if (player) {
        if (pressed) player->moveRight();
        else player->stopMovement();
    }
}

void APIBridge::playerMoveUp(bool pressed) {
    auto player = GameManager::getInstance()->getPlayer();
    if (player) {
        if (pressed) player->moveUp();
        else player->stopMovement();
    }
}

void APIBridge::playerMoveDown(bool pressed) {
    auto player = GameManager::getInstance()->getPlayer();
    if (player) {
        if (pressed) player->moveDown();
        else player->stopMovement();
    }
}

void APIBridge::playerShoot() {
    auto player = GameManager::getInstance()->getPlayer();
    auto level = GameManager::getInstance()->getCurrentLevel();
    
    if (player && level && player->canShoot()) {
        player->shoot();
        level->playerShoot(player->getPosition());
        sendEvent("player_shot");
    }
}

void APIBridge::playerSpecialAttack() {
    // TODO: Implement special attack logic
    sendEvent("special_attack");
}

// ===== GAME LOOP =====

void APIBridge::update(float deltaTime) {
    GameManager::getInstance()->update();
}

// ===== RENDER DATA =====

std::vector<EntityRenderData> APIBridge::getRenderData() {
    std::vector<EntityRenderData> renderData;
    auto gm = GameManager::getInstance();
    auto player = gm->getPlayer();
    auto level = gm->getCurrentLevel();
    auto world = gm->getCurrentWorld();
    auto assets = AssetRegistry::getInstance();
    
    if (!player || !level || !world) {
        return renderData;
    }
    
    std::string worldName = world->getName();
    
    // Player
    if (player->isActive()) {
        EntityRenderData playerData;
        playerData.id = "player";
        playerData.type = "player";
        playerData.assetPath = assets->getPlayerAsset(player->getCharacterId());
        playerData.x = player->getPosition().x;
        playerData.y = player->getPosition().y;
        playerData.width = player->getWidth();
        playerData.height = player->getHeight();
        playerData.rotation = 0;
        playerData.scale = 1.0f;
        playerData.visible = true;
        playerData.layer = 5;
        renderData.push_back(playerData);
    }
    
    // Enemies
    const auto& enemies = level->getEnemies();
    int enemyIndex = 0;
    for (const auto& enemy : enemies) {
        if (enemy->isActive()) {
            EntityRenderData enemyData;
            enemyData.id = "enemy_" + std::to_string(enemyIndex++);
            enemyData.type = "enemy";
            
            // Get appropriate enemy asset
            int assetIndex = rand() % assets->getEnemyAssets(worldName).size();
            enemyData.assetPath = assets->getEnemyAsset(worldName, assetIndex);
            
            enemyData.x = enemy->getPosition().x;
            enemyData.y = enemy->getPosition().y;
            enemyData.width = enemy->getWidth();
            enemyData.height = enemy->getHeight();
            enemyData.rotation = 0;
            enemyData.scale = 1.0f;
            enemyData.visible = true;
            enemyData.layer = 4;
            renderData.push_back(enemyData);
        }
    }
    
    // Player Bullets
    const auto& playerBullets = level->getPlayerBullets();
    int bulletIndex = 0;
    for (const auto& bullet : playerBullets) {
        if (bullet->isActive()) {
            EntityRenderData bulletData;
            bulletData.id = "pbullet_" + std::to_string(bulletIndex++);
            bulletData.type = "player_bullet";
            bulletData.assetPath = assets->getPlayerBulletAsset(worldName);
            bulletData.x = bullet->getPosition().x;
            bulletData.y = bullet->getPosition().y;
            bulletData.width = bullet->getWidth();
            bulletData.height = bullet->getHeight();
            bulletData.rotation = 0;
            bulletData.scale = 1.0f;
            bulletData.visible = true;
            bulletData.layer = 6;
            renderData.push_back(bulletData);
        }
    }
    
    // Enemy Bullets
    const auto& enemyBullets = level->getEnemyBullets();
    int eBulletIndex = 0;
    for (const auto& bullet : enemyBullets) {
        if (bullet->isActive()) {
            EntityRenderData bulletData;
            bulletData.id = "ebullet_" + std::to_string(eBulletIndex++);
            bulletData.type = "enemy_bullet";
            bulletData.assetPath = assets->getEnemyBulletAsset(worldName);
            bulletData.x = bullet->getPosition().x;
            bulletData.y = bullet->getPosition().y;
            bulletData.width = bullet->getWidth();
            bulletData.height = bullet->getHeight();
            bulletData.rotation = 0;
            bulletData.scale = 1.0f;
            bulletData.visible = true;
            bulletData.layer = 6;
            renderData.push_back(bulletData);
        }
    }
    
    // Bonuses
    const auto& bonuses = level->getBonuses();
    int bonusIndex = 0;
    for (const auto& bonus : bonuses) {
        if (bonus->isActive()) {
            EntityRenderData bonusData;
            bonusData.id = "bonus_" + std::to_string(bonusIndex++);
            bonusData.type = "bonus";
            bonusData.assetPath = assets->getBonusAsset("shield"); // TODO: Map bonus type
            bonusData.x = bonus->getPosition().x;
            bonusData.y = bonus->getPosition().y;
            bonusData.width = bonus->getWidth();
            bonusData.height = bonus->getHeight();
            bonusData.rotation = 0;
            bonusData.scale = 1.0f;
            bonusData.visible = true;
            bonusData.layer = 5;
            renderData.push_back(bonusData);
        }
    }
    
    // Explosions
    const auto& explosions = level->getExplosions();
    int explosionIndex = 0;
    for (const auto& explosion : explosions) {
        if (explosion->isActive()) {
            EntityRenderData explosionData;
            explosionData.id = "explosion_" + std::to_string(explosionIndex++);
            explosionData.type = "explosion";
            explosionData.assetPath = assets->getEnemyExplosionAsset(worldName);
            explosionData.x = explosion->getPosition().x;
            explosionData.y = explosion->getPosition().y;
            explosionData.width = 60;
            explosionData.height = 60;
            explosionData.rotation = 0;
            explosionData.scale = explosion->getScale();
            explosionData.visible = true;
            explosionData.layer = 7;
            renderData.push_back(explosionData);
        }
    }
    
    return renderData;
}

std::string APIBridge::getCurrentBackground() {
    auto world = GameManager::getInstance()->getCurrentWorld();
    if (world) {
        return AssetRegistry::getInstance()->getWorldBackground(world->getName());
    }
    return "";
}

GameStateData APIBridge::getGameState() {
    GameStateData data;
    auto gm = GameManager::getInstance();
    auto player = gm->getPlayer();
    auto level = gm->getCurrentLevel();
    auto world = gm->getCurrentWorld();
    
    data.state = std::to_string(static_cast<int>(gm->getState()));
    data.playerLives = player ? player->getLives() : 0;
    data.playerScore = player ? player->getScore() : 0;
    data.currentLevel = level ? level->getLevelNumber() : 0;
    data.currentWorld = world ? world->getName() : "";
    data.enemiesRemaining = level ? level->getRemainingEnemies() : 0;
    data.isPaused = (gm->getState() == GameState::PAUSE);
    data.isGameOver = (gm->getState() == GameState::GAME_OVER);
    data.isLevelComplete = level ? level->isCompleted() : false;
    
    return data;
}

// ===== JSON EXPORT =====

std::string APIBridge::getRenderDataJSON() {
    auto renderData = getRenderData();
    std::stringstream json;
    
    json << "{\"entities\":[";
    for (size_t i = 0; i < renderData.size(); ++i) {
        const auto& entity = renderData[i];
        json << "{";
        json << "\"id\":\"" << entity.id << "\",";
        json << "\"type\":\"" << entity.type << "\",";
        json << "\"asset\":\"" << entity.assetPath << "\",";
        json << "\"x\":" << entity.x << ",";
        json << "\"y\":" << entity.y << ",";
        json << "\"width\":" << entity.width << ",";
        json << "\"height\":" << entity.height << ",";
        json << "\"rotation\":" << entity.rotation << ",";
        json << "\"scale\":" << entity.scale << ",";
        json << "\"visible\":" << (entity.visible ? "true" : "false") << ",";
        json << "\"layer\":" << entity.layer;
        json << "}";
        if (i < renderData.size() - 1) json << ",";
    }
    json << "],";
    json << "\"background\":\"" << getCurrentBackground() << "\"";
    json << "}";
    
    return json.str();
}

std::string APIBridge::getGameStateJSON() {
    auto state = getGameState();
    std::stringstream json;
    
    json << "{";
    json << "\"state\":\"" << state.state << "\",";
    json << "\"lives\":" << state.playerLives << ",";
    json << "\"score\":" << state.playerScore << ",";
    json << "\"level\":" << state.currentLevel << ",";
    json << "\"world\":\"" << state.currentWorld << "\",";
    json << "\"enemiesRemaining\":" << state.enemiesRemaining << ",";
    json << "\"paused\":" << (state.isPaused ? "true" : "false") << ",";
    json << "\"gameOver\":" << (state.isGameOver ? "true" : "false") << ",";
    json << "\"levelComplete\":" << (state.isLevelComplete ? "true" : "false");
    json << "}";
    
    return json.str();
}

std::string APIBridge::getAllAssetsJSON() {
    auto assets = AssetRegistry::getInstance()->getAllAssetPaths();
    std::stringstream json;
    
    json << "{\"assets\":[";
    for (size_t i = 0; i < assets.size(); ++i) {
        json << "\"" << assets[i] << "\"";
        if (i < assets.size() - 1) json << ",";
    }
    json << "]}";
    
    return json.str();
}

// ===== UTILITY =====

void APIBridge::log(const std::string& message, const std::string& level) {
    std::cout << "[" << level << "] " << message << std::endl;
    if (logCallback) {
        logCallback("[" + level + "] " + message);
    }
}

void APIBridge::sendEvent(const std::string& eventType, const std::string& data) {
    if (eventCallback) {
        std::string event = "{\"type\":\"" + eventType + "\",\"data\":\"" + data + "\"}";
        eventCallback(event);
    }
}

bool APIBridge::isInitialized() const {
    return GameManager::getInstance() != nullptr;
}

float APIBridge::getFPS() const {
    return 60.0f; // TODO: Calculate actual FPS
}

std::vector<std::string> APIBridge::getAllAssetPaths() {
    return AssetRegistry::getInstance()->getAllAssetPaths();
}

std::string APIBridge::getPlayerAsset(int characterId) {
    return AssetRegistry::getInstance()->getPlayerAsset(characterId);
}

std::string APIBridge::getWorldBackground(const std::string& world) {
    return AssetRegistry::getInstance()->getWorldBackground(world);
}

// ===== C-STYLE API =====

static std::string lastJSONResult;

extern "C" {
    void API_InitGame(int width, int height) {
        APIBridge::getInstance()->initGame(width, height);
    }
    
    void API_CreatePlayer(int characterId) {
        APIBridge::getInstance()->createPlayer(characterId);
    }
    
    void API_SelectWorld(const char* worldName) {
        APIBridge::getInstance()->selectWorld(std::string(worldName));
    }
    
    void API_StartLevel(int levelNumber) {
        APIBridge::getInstance()->startLevel(levelNumber);
    }
    
    void API_PauseGame() {
        APIBridge::getInstance()->pauseGame();
    }
    
    void API_ResumeGame() {
        APIBridge::getInstance()->resumeGame();
    }
    
    void API_RestartLevel() {
        APIBridge::getInstance()->restartLevel();
    }
    
    void API_NextLevel() {
        APIBridge::getInstance()->nextLevel();
    }
    
    void API_QuitToMenu() {
        APIBridge::getInstance()->quitToMenu();
    }
    
    void API_PlayerMoveLeft(bool pressed) {
        APIBridge::getInstance()->playerMoveLeft(pressed);
    }
    
    void API_PlayerMoveRight(bool pressed) {
        APIBridge::getInstance()->playerMoveRight(pressed);
    }
    
    void API_PlayerMoveUp(bool pressed) {
        APIBridge::getInstance()->playerMoveUp(pressed);
    }
    
    void API_PlayerMoveDown(bool pressed) {
        APIBridge::getInstance()->playerMoveDown(pressed);
    }
    
    void API_PlayerShoot() {
        APIBridge::getInstance()->playerShoot();
    }
    
    void API_PlayerSpecialAttack() {
        APIBridge::getInstance()->playerSpecialAttack();
    }
    
    void API_Update(float deltaTime) {
        APIBridge::getInstance()->update(deltaTime);
    }
    
    const char* API_GetRenderDataJSON() {
        lastJSONResult = APIBridge::getInstance()->getRenderDataJSON();
        return lastJSONResult.c_str();
    }
    
    const char* API_GetGameStateJSON() {
        lastJSONResult = APIBridge::getInstance()->getGameStateJSON();
        return lastJSONResult.c_str();
    }
    
    const char* API_GetAllAssetsJSON() {
        lastJSONResult = APIBridge::getInstance()->getAllAssetsJSON();
        return lastJSONResult.c_str();
    }
    
    int API_GetPlayerLives() {
        return APIBridge::getInstance()->getGameState().playerLives;
    }
    
    int API_GetPlayerScore() {
        return APIBridge::getInstance()->getGameState().playerScore;
    }
    
    int API_GetCurrentLevel() {
        return APIBridge::getInstance()->getGameState().currentLevel;
    }
    
    const char* API_GetCurrentWorld() {
        static std::string worldName;
        worldName = APIBridge::getInstance()->getGameState().currentWorld;
        return worldName.c_str();
    }
    
    bool API_IsGameOver() {
        return APIBridge::getInstance()->getGameState().isGameOver;
    }
    
    bool API_IsLevelComplete() {
        return APIBridge::getInstance()->getGameState().isLevelComplete;
    }
    
    bool API_IsPaused() {
        return APIBridge::getInstance()->getGameState().isPaused;
    }
    
    const char* API_GetPlayerAsset(int characterId) {
        static std::string assetPath;
        assetPath = APIBridge::getInstance()->getPlayerAsset(characterId);
        return assetPath.c_str();
    }
    
    const char* API_GetWorldBackground(const char* worldName) {
        static std::string bgPath;
        bgPath = APIBridge::getInstance()->getWorldBackground(std::string(worldName));
        return bgPath.c_str();
    }
}
