#ifndef GAME_MANAGER_HPP
#define GAME_MANAGER_HPP

#include "Constants.hpp"
#include "World.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include <map>
#include <memory>
#include <string>

class GameManager {
private:
    static GameManager* instance;
    
    // Game state
    GameState currentState;
    GameState previousState;
    
    // Worlds and levels
    std::map<std::string, std::shared_ptr<World>> worlds;
    std::shared_ptr<World> currentWorld;
    std::shared_ptr<Level> currentLevel;
    
    // Player
    std::shared_ptr<Player> player;
    
    // Game session data
    std::string currentUser;
    bool isGuest;
    int highScore;
    unsigned int sessionStartTime;
    
    // Screen dimensions
    int screenWidth;
    int screenHeight;
    
    // Timing
    unsigned int lastUpdateTime;
    float deltaTime;
    
    GameManager();
    
public:
    static GameManager* getInstance();
    ~GameManager() = default;
    
    // Initialization
    void initialize(int screenWidth, int screenHeight);
    void loadWorlds();
    void createWorld(const std::string& name, const WorldTheme& theme);
    
    // Game loop
    void update();
    void render();
    
    // State management
    void setState(GameState newState);
    GameState getState() const { return currentState; }
    GameState getPreviousState() const { return previousState; }
    
    // World and level management
    void selectWorld(const std::string& worldName);
    void startLevel(int levelNumber);
    void restartLevel();
    void nextLevel();
    bool hasNextLevel() const;
    
    // Player management
    void createPlayer(int characterId);
    std::shared_ptr<Player> getPlayer() { return player; }
    void setUser(const std::string& username, bool guest = false);
    std::string getCurrentUser() const { return currentUser; }
    bool isGuestMode() const { return isGuest; }
    
    // Score management
    void updateHighScore();
    int getHighScore() const { return highScore; }
    void saveScore();
    
    // Screen
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    void setScreenSize(int width, int height);
    
    // Level access
    std::shared_ptr<Level> getCurrentLevel() { return currentLevel; }
    std::shared_ptr<World> getCurrentWorld() { return currentWorld; }
    std::map<std::string, std::shared_ptr<World>>& getWorlds() { return worlds; }
    
    // Timing
    float getDeltaTime() const { return deltaTime; }
    unsigned int getCurrentTime() const;
    
    // Game flow
    void gameOver();
    void levelWin();
    void pauseGame();
    void resumeGame();
    void quitToMenu();
    
    // Cleanup
    void cleanup();
    void reset();
};

#endif // GAME_MANAGER_HPP
