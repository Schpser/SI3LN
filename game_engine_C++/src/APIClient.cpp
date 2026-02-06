#include "APIClient.h"
#include <iostream>
#include <sstream>

using json = nlohmann::json;

namespace SI3LN {

APIClient::APIClient(const std::string& baseUrl) : baseUrl_(baseUrl), playerId_(-1) {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_ = curl_easy_init();
}

APIClient::~APIClient() {
    if (curl_) {
        curl_easy_cleanup(curl_);
    }
    curl_global_cleanup();
}

size_t APIClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string APIClient::performRequest(const std::string& endpoint, const std::string& method, 
                                      const std::string& jsonData, bool requiresAuth) {
    if (!curl_) {
        std::cerr << "CURL not initialized" << std::endl;
        return "";
    }

    std::string url = baseUrl_ + endpoint;
    std::string responseString;
    
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &responseString);
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    // Add Authorization header if authenticated
    if (requiresAuth && !jwtToken_.empty()) {
        std::string authHeader = "Authorization: Bearer " + jwtToken_;
        headers = curl_slist_append(headers, authHeader.c_str());
    }
    
    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, headers);
    
    if (method == "POST") {
        curl_easy_setopt(curl_, CURLOPT_POST, 1L);
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, jsonData.c_str());
    } else if (method == "PATCH") {
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, jsonData.c_str());
    } else if (method == "GET") {
        curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
    }
    
    CURLcode res = curl_easy_perform(curl_);
    
    if (res != CURLE_OK) {
        std::cerr << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
    }
    
    curl_slist_free_all(headers);
    
    return responseString;
}

bool APIClient::login(const std::string& username, const std::string& password) {
    json data;
    data["username"] = username;
    data["password"] = password;
    
    std::string response = performRequest("/auth/login", "POST", data.dump(), false);
    
    try {
        json responseJson = json::parse(response);
        if (responseJson.contains("token")) {
            jwtToken_ = responseJson["token"];
            playerId_ = responseJson["player_id"];
            std::cout << "[API] Logged in successfully. Token expires in 24 hours." << std::endl;
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse login response: " << e.what() << std::endl;
    }
    return false;
}

bool APIClient::registerUser(const std::string& username, const std::string& password, const std::string& email) {
    json data;
    data["username"] = username;
    data["password"] = password;
    if (!email.empty()) {
        data["email"] = email;
    }
    
    std::string response = performRequest("/auth/register", "POST", data.dump(), false);
    
    try {
        json responseJson = json::parse(response);
        if (responseJson.contains("token")) {
            jwtToken_ = responseJson["token"];
            playerId_ = responseJson["player_id"];
            std::cout << "[API] Registered successfully. Token expires in 24 hours." << std::endl;
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse register response: " << e.what() << std::endl;
    }
    return false;
}

void APIClient::logout() {
    jwtToken_.clear();
    playerId_ = -1;
    std::cout << "[API] Logged out." << std::endl;
}

int APIClient::createPlayer(const std::string& username, const std::string& email) {
    json data;
    data["username"] = username;
    data["email"] = email;
    
    std::string response = performRequest("/game/players", "POST", data.dump(), false);
    
    try {
        json responseJson = json::parse(response);
        return responseJson["id"];
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse player creation response: " << e.what() << std::endl;
        return -1;
    }
}

int APIClient::startGameSession(int playerId, int worldId) {
    json data;
    data["player_id"] = playerId;
    if (worldId > 0) {
        data["world_id"] = worldId;
    }
    
    std::string response = performRequest("/game/sessions", "POST", data.dump(), true);
    
    try {
        json responseJson = json::parse(response);
        return responseJson["id"];
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse session creation response: " << e.what() << std::endl;
        return -1;
    }
}

bool APIClient::updateGameSession(int sessionId, int score, int level, int enemiesKilled, int duration) {
    json data;
    data["score"] = score;
    data["level_reached"] = level;
    data["enemies_killed"] = enemiesKilled;
    data["duration_seconds"] = duration;
    
    std::string endpoint = "/sessions/" + std::to_string(sessionId);
    std::string response = performRequest(endpoint, "PATCH", data.dump());
    
    return !response.empty();
}

bool APIClient::endGameSession(int sessionId, int finalScore, int level, bool completed) {
    json data;
    data["score"] = finalScore;
    data["level_reached"] = level;
    data["completed"] = completed;
    data["ended_at"] = ""; // Server will use current time if empty
    
    std::string endpoint = "/sessions/" + std::to_string(sessionId);
    std::string response = performRequest(endpoint, "PATCH", data.dump());
    
    return !response.empty();
}

json APIClient::getLeaderboard(int limit) {
    std::string endpoint = "/game/leaderboard?limit=" + std::to_string(limit);
    std::string response = performRequest(endpoint, "GET", "", false);
    
    try {
        return json::parse(response);
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse leaderboard: " << e.what() << std::endl;
        return json::array();
    }
}

json APIClient::getStats() {
    std::string response = performRequest("/game/stats", "GET", "", false);
    
    try {
        return json::parse(response);
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse stats: " << e.what() << std::endl;
        return json::object();
    }
}

} // namespace SI3LN
