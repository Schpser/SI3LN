#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <string>
#ifndef __EMSCRIPTEN__
#include <curl/curl.h>
#endif
#include <nlohmann/json.hpp>

namespace SI3LN {

class APIClient {
public:
    APIClient(const std::string& baseUrl = "http://127.0.0.1:8000/api");
    ~APIClient();

    // Authentication operations
    bool login(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password, const std::string& email = "");
    void logout();
    bool isAuthenticated() const { return !jwtToken_.empty(); }
    
    // Player operations (requires authentication)
    int createPlayer(const std::string& username, const std::string& email);
    
    // Game session operations (requires authentication)
    int startGameSession(int playerId, int worldId = 0);
    bool updateGameSession(int sessionId, int score, int level, int enemiesKilled, int duration);
    bool endGameSession(int sessionId, int finalScore, int level, bool completed);
    
    // Public operations (no authentication required)
    nlohmann::json getLeaderboard(int limit = 10);
    nlohmann::json getStats();

private:
    std::string baseUrl_;
    std::string jwtToken_; // JWT token for authentication
    int playerId_; // Current player ID
#ifndef __EMSCRIPTEN__
    CURL* curl_;
#endif

    // Helper
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    std::string performRequest(const std::string& endpoint, const std::string& method, 
                               const std::string& jsonData = "", bool requiresAuth = true);
};

} // namespace SI3LN

#endif // API_CLIENT_H
