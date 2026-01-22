#ifndef API_BRIDGE_HPP
#define API_BRIDGE_HPP

#include <string>
#include <vector>
#include <functional>

/**
 * API Bridge - Communication Layer between C++ Engine and Frontend
 * 
 * This class provides a clean interface for the frontend (HTML/CSS/JS) to:
 * 1. Send commands to the C++ engine
 * 2. Query game state
 * 3. Get entity positions and assets for rendering
 * 
 * The bridge uses JSON for data exchange (can be easily consumed by JavaScript)
 */

// Entity render data structure
struct EntityRenderData {
    std::string id;           // Unique entity ID
    std::string type;         // "player", "enemy", "bullet", etc.
    std::string assetPath;    // Path to the sprite/image
    float x, y;               // Position
    float width, height;      // Dimensions
    float rotation;           // Rotation angle (degrees)
    float scale;              // Scale factor
    bool visible;             // Visibility flag
    int layer;                // Render layer (0=background, 10=foreground)
};

// Game state data
struct GameStateData {
    std::string state;        // Current game state
    int playerLives;          // Player lives
    int playerScore;          // Player score
    int currentLevel;         // Current level number
    std::string currentWorld; // Current world name
    int enemiesRemaining;     // Enemies left
    bool isPaused;            // Pause state
    bool isGameOver;          // Game over flag
    bool isLevelComplete;     // Level completion flag
};

class APIBridge {
private:
    static APIBridge* instance;
    
    // Callbacks for frontend communication
    std::function<void(const std::string&)> logCallback;
    std::function<void(const std::string&)> eventCallback;
    
    APIBridge();
    
public:
    static APIBridge* getInstance();
    ~APIBridge() = default;
    
    // ===== INITIALIZATION =====
    void initialize();
    void setLogCallback(std::function<void(const std::string&)> callback);
    void setEventCallback(std::function<void(const std::string&)> callback);
    
    // ===== GAME CONTROL COMMANDS =====
    void initGame(int screenWidth, int screenHeight);
    void createPlayer(int characterId);
    void selectWorld(const std::string& worldName);
    void startLevel(int levelNumber);
    void pauseGame();
    void resumeGame();
    void restartLevel();
    void nextLevel();
    void quitToMenu();
    void exitGame();
    
    // ===== PLAYER CONTROLS =====
    void playerMoveLeft(bool pressed);
    void playerMoveRight(bool pressed);
    void playerMoveUp(bool pressed);
    void playerMoveDown(bool pressed);
    void playerShoot();
    void playerSpecialAttack();
    
    // ===== GAME LOOP =====
    void update(float deltaTime);
    
    // ===== RENDER DATA QUERIES =====
    // Get all entities that need to be rendered
    std::vector<EntityRenderData> getRenderData();
    
    // Get background for current state
    std::string getCurrentBackground();
    
    // Get UI data
    GameStateData getGameState();
    
    // ===== ASSET QUERIES =====
    // Get all asset paths (for preloading)
    std::vector<std::string> getAllAssetPaths();
    
    // Get specific assets
    std::string getPlayerAsset(int characterId);
    std::string getWorldBackground(const std::string& world);
    
    // ===== JSON EXPORT =====
    // Export data as JSON string for JavaScript consumption
    std::string getRenderDataJSON();
    std::string getGameStateJSON();
    std::string getAllAssetsJSON();
    
    // ===== EVENTS =====
    // Send log message to frontend
    void log(const std::string& message, const std::string& level = "info");
    
    // Send game event to frontend
    void sendEvent(const std::string& eventType, const std::string& data = "");
    
    // ===== UTILITY =====
    // Check if game is initialized
    bool isInitialized() const;
    
    // Get FPS
    float getFPS() const;
};

// ===== C-STYLE API FOR EASIER INTEGRATION =====
// These functions can be easily called from JavaScript via Emscripten
extern "C" {
    // Initialization
    void API_InitGame(int width, int height);
    void API_CreatePlayer(int characterId);
    void API_SelectWorld(const char* worldName);
    void API_StartLevel(int levelNumber);
    
    // Game control
    void API_PauseGame();
    void API_ResumeGame();
    void API_RestartLevel();
    void API_NextLevel();
    void API_QuitToMenu();
    
    // Player controls
    void API_PlayerMoveLeft(bool pressed);
    void API_PlayerMoveRight(bool pressed);
    void API_PlayerMoveUp(bool pressed);
    void API_PlayerMoveDown(bool pressed);
    void API_PlayerShoot();
    void API_PlayerSpecialAttack();
    
    // Game loop
    void API_Update(float deltaTime);
    
    // Data queries
    const char* API_GetRenderDataJSON();
    const char* API_GetGameStateJSON();
    const char* API_GetAllAssetsJSON();
    
    // State queries
    int API_GetPlayerLives();
    int API_GetPlayerScore();
    int API_GetCurrentLevel();
    const char* API_GetCurrentWorld();
    bool API_IsGameOver();
    bool API_IsLevelComplete();
    bool API_IsPaused();
    
    // Asset queries
    const char* API_GetPlayerAsset(int characterId);
    const char* API_GetWorldBackground(const char* worldName);
}

#endif // API_BRIDGE_HPP
