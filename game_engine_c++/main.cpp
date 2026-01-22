/**
 * SI3LN - Space Invaders III Last Night
 * C++ Game Engine - Main Entry Point
 * 
 * This is the core game logic engine. UI is handled by HTML/CSS/JavaScript.
 * The C++ engine communicates with the frontend through a defined API.
 */

#include "include/GameManager.hpp"
#include "include/APIBridge.hpp"
#include "include/AssetRegistry.hpp"
#include "include/Constants.hpp"
#include "include/Utils.hpp"
#include <iostream>
#include <string>
#include <csignal>

// Global flag for graceful shutdown
bool running = true;

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down gracefully..." << std::endl;
    running = false;
}

void printBanner() {
    std::cout << "========================================" << std::endl;
    std::cout << "   SI3LN - Space Invaders III Last Night" << std::endl;
    std::cout << "   C++ Game Engine v1.0" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
}

void printUsage() {
    std::cout << "Usage: game_engine [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --width <width>      Set screen width (default: 1024)" << std::endl;
    std::cout << "  --height <height>    Set screen height (default: 768)" << std::endl;
    std::cout << "  --help               Show this help message" << std::endl;
    std::cout << std::endl;
}

// API Functions for Frontend Integration
extern "C" {
    // Game initialization
    void initGame(int width, int height) {
        GameManager* gm = GameManager::getInstance();
        gm->initialize(width, height);
    }
    
    // Player actions
    void createPlayer(int characterId) {
        GameManager* gm = GameManager::getInstance();
        gm->createPlayer(characterId);
    }
    
    void playerMoveLeft() {
        GameManager* gm = GameManager::getInstance();
        auto player = gm->getPlayer();
        if (player) player->moveLeft();
    }
    
    void playerMoveRight() {
        GameManager* gm = GameManager::getInstance();
        auto player = gm->getPlayer();
        if (player) player->moveRight();
    }
    
    void playerMoveUp() {
        GameManager* gm = GameManager::getInstance();
        auto player = gm->getPlayer();
        if (player) player->moveUp();
    }
    
    void playerMoveDown() {
        GameManager* gm = GameManager::getInstance();
        auto player = gm->getPlayer();
        if (player) player->moveDown();
    }
    
    void playerStopMovement() {
        GameManager* gm = GameManager::getInstance();
        auto player = gm->getPlayer();
        if (player) player->stopMovement();
    }
    
    void playerShoot() {
        GameManager* gm = GameManager::getInstance();
        auto player = gm->getPlayer();
        auto level = gm->getCurrentLevel();
        
        if (player && level && player->canShoot()) {
            player->shoot();
            level->playerShoot(player->getPosition());
        }
    }
    
    // World and level management
    void selectWorld(const char* worldName) {
        GameManager* gm = GameManager::getInstance();
        gm->selectWorld(std::string(worldName));
    }
    
    void startLevel(int levelNumber) {
        GameManager* gm = GameManager::getInstance();
        gm->startLevel(levelNumber);
    }
    
    void nextLevel() {
        GameManager* gm = GameManager::getInstance();
        gm->nextLevel();
    }
    
    void restartLevel() {
        GameManager* gm = GameManager::getInstance();
        gm->restartLevel();
    }
    
    // Game state
    void pauseGame() {
        GameManager* gm = GameManager::getInstance();
        gm->pauseGame();
    }
    
    void resumeGame() {
        GameManager* gm = GameManager::getInstance();
        gm->resumeGame();
    }
    
    void quitToMenu() {
        GameManager* gm = GameManager::getInstance();
        gm->quitToMenu();
    }
    
    // Game loop
    void updateGame() {
        GameManager* gm = GameManager::getInstance();
        gm->update();
    }
    
    void renderGame() {
        GameManager* gm = GameManager::getInstance();
        gm->render();
    }
    
    // User management
    void setUser(const char* username, bool isGuest) {
        GameManager* gm = GameManager::getInstance();
        gm->setUser(std::string(username), isGuest);
    }
    
    // Getters for frontend
    int getPlayerLives() {
        GameManager* gm = GameManager::getInstance();
        auto player = gm->getPlayer();
        return player ? player->getLives() : 0;
    }
    
    int getPlayerScore() {
        GameManager* gm = GameManager::getInstance();
        auto player = gm->getPlayer();
        return player ? player->getScore() : 0;
    }
    
    int getCurrentState() {
        GameManager* gm = GameManager::getInstance();
        return static_cast<int>(gm->getState());
    }
    
    bool isLevelCompleted() {
        GameManager* gm = GameManager::getInstance();
        auto level = gm->getCurrentLevel();
        return level ? level->isCompleted() : false;
    }
}

int main(int argc, char* argv[]) {
    // Setup signal handlers for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    printBanner();
    
    // Parse command line arguments
    int screenWidth = Constants::DEFAULT_SCREEN_WIDTH;
    int screenHeight = Constants::DEFAULT_SCREEN_HEIGHT;
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--help" || arg == "-h") {
            printUsage();
            return 0;
        } else if (arg == "--width" && i + 1 < argc) {
            screenWidth = std::stoi(argv[++i]);
        } else if (arg == "--height" && i + 1 < argc) {
            screenHeight = std::stoi(argv[++i]);
        }
    }
    
    // Initialize game
    std::cout << "Initializing game engine..." << std::endl;
    GameManager* gameManager = GameManager::getInstance();
    gameManager->initialize(screenWidth, screenHeight);
    
    std::cout << "Game engine initialized successfully!" << std::endl;
    std::cout << "Screen resolution: " << screenWidth << "x" << screenHeight << std::endl;
    std::cout << std::endl;
    
    // Main game loop (for standalone testing)
    std::cout << "Starting main game loop..." << std::endl;
    std::cout << "Press Ctrl+C to exit" << std::endl;
    std::cout << std::endl;
    
    // For testing: create a player and start level 1
    gameManager->createPlayer(0);
    gameManager->selectWorld("Space");
    gameManager->startLevel(1);
    
    unsigned int frameCount = 0;
    unsigned int lastFpsTime = Utils::getTimeMs();
    float fps = 0.0f;
    
    while (running) {
        // Update game logic
        gameManager->update();
        
        // Render (in a real implementation, this would output to graphics system)
        // gameManager->render();
        
        frameCount++;
        unsigned int currentTime = Utils::getTimeMs();
        if (currentTime - lastFpsTime >= 1000) {
            fps = frameCount * 1000.0f / (currentTime - lastFpsTime);
            frameCount = 0;
            lastFpsTime = currentTime;
            
            auto player = gameManager->getPlayer();
            if (player) {
                std::cout << "FPS: " << fps 
                         << " | Lives: " << player->getLives() 
                         << " | Score: " << player->getScore() 
                         << std::endl;
            }
        }
        
        // Cap frame rate
        Utils::sleep(1000 / Constants::FPS);
        
        // Check win/loss conditions
        if (gameManager->getState() == GameState::GAME_OVER) {
            std::cout << "Game Over! Final score: " << gameManager->getPlayer()->getScore() << std::endl;
            break;
        }
        
        if (gameManager->getState() == GameState::LEVEL_WIN) {
            auto level = gameManager->getCurrentLevel();
            std::cout << "Level " << level->getLevelNumber() << " completed!" << std::endl;
            
            if (gameManager->hasNextLevel()) {
                std::cout << "Starting next level..." << std::endl;
                gameManager->nextLevel();
            } else {
                std::cout << "All levels completed!" << std::endl;
                break;
            }
        }
    }
    
    // Cleanup
    std::cout << std::endl;
    std::cout << "Shutting down game engine..." << std::endl;
    gameManager->cleanup();
    
    std::cout << "Goodbye!" << std::endl;
    return 0;
}
