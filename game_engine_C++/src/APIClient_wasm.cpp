/**
 * APIClient_wasm.cpp
 * Emscripten/WebAssembly replacement for APIClient.cpp
 *
 * Uses the browser's native fetch() via EM_ASYNC_JS instead of libcurl.
 * The base URL is relative (/api) so it always hits the same domain,
 * meaning nginx on port 80 proxies it correctly to the Django container.
 *
 * Build requirement: -sASYNCIFY=1
 */
#ifdef __EMSCRIPTEN__

#include "APIClient.h"
#include <emscripten.h>
#include <emscripten/fetch.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace SI3LN {

// ---------------------------------------------------------------------------
// JavaScript helper: performs a fetch() call and returns the response body
// as a malloc'd C string (the caller must free it).
// Returns nullptr on network error.
// ---------------------------------------------------------------------------
EM_ASYNC_JS(char*, js_fetch, (const char* method, const char* url,
                               const char* body, const char* auth_token), {
    const methodStr    = UTF8ToString(method);
    const urlStr       = UTF8ToString(url);
    const bodyStr      = body   ? UTF8ToString(body)       : null;
    const tokenStr     = auth_token ? UTF8ToString(auth_token) : null;

    const headers = { "Content-Type": "application/json" };
    if (tokenStr) {
        headers["Authorization"] = "Bearer " + tokenStr;
    }

    const opts = {
        method  : methodStr,
        headers : headers,
    };
    if (bodyStr && methodStr !== "GET") {
        opts.body = bodyStr;
    }

    try {
        const response = await fetch(urlStr, opts);
        const text = await response.text();
        // Allocate WASM memory and copy the string
        const len = lengthBytesUTF8(text) + 1;
        const buf = _malloc(len);
        stringToUTF8(text, buf, len);
        return buf;
    } catch (e) {
        console.error("[APIClient WASM] fetch error:", e);
        return 0; // nullptr
    }
});

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------
APIClient::APIClient(const std::string& baseUrl)
    : baseUrl_("/api"), playerId_(-1)
{
    // Try to inherit the logged-in JWT from the parent dashboard page.
    // game/index.html is same-origin, so it can read window.parent.localStorage.
    char* raw = (char*)EM_ASM_PTR({
        try {
            var token = window.SI3LN_JWT_TOKEN
                     || (window.parent && window.parent.localStorage
                         ? window.parent.localStorage.getItem('access_token')
                         : null)
                     || "";
            var len = lengthBytesUTF8(token) + 1;
            var buf = _malloc(len);
            stringToUTF8(token, buf, len);
            return buf;
        } catch(e) { return 0; }
    });

    if (raw && strlen(raw) > 0) {
        jwtToken_ = std::string(raw);
        // Decode player_id from JWT payload (base64url middle segment)
        try {
            std::string tok = jwtToken_;
            size_t d1 = tok.find('.');
            size_t d2 = (d1 != std::string::npos) ? tok.find('.', d1+1) : std::string::npos;
            if (d1 != std::string::npos && d2 != std::string::npos) {
                std::string payload = tok.substr(d1+1, d2-d1-1);
                // Add padding
                while (payload.size() % 4) payload += '=';
                // base64url → base64
                for (char& c : payload) {
                    if (c == '-') c = '+';
                    else if (c == '_') c = '/';
                }
                std::string decoded = EM_ASM_PTR({
                    return 0; // Let JSON decoder in JS handle it
                }) ? "" : "";
                // Simpler: let js_fetch with /auth/me set player_id
                // We'll resolve player_id lazily when needed
            }
        } catch (...) {}
        std::cout << "[APIClient WASM] JWT inherited from parent session." << std::endl;
    }
    if (raw) free(raw);
    std::cout << "[APIClient WASM] Initialised. Authenticated: "
              << (isAuthenticated() ? "yes" : "no") << std::endl;
}

APIClient::~APIClient() {}

// ---------------------------------------------------------------------------
// Internal helper: calls js_fetch and returns response as std::string
// ---------------------------------------------------------------------------
std::string APIClient::performRequest(const std::string& endpoint,
                                       const std::string& method,
                                       const std::string& jsonData,
                                       bool requiresAuth)
{
    std::string url = baseUrl_ + endpoint;
    const char* body  = jsonData.empty() ? nullptr : jsonData.c_str();
    const char* token = (requiresAuth && !jwtToken_.empty())
                            ? jwtToken_.c_str() : nullptr;

    char* raw = js_fetch(method.c_str(), url.c_str(), body, token);
    if (!raw) return "";

    std::string result(raw);
    free(raw);
    return result;
}

// WriteCallback is unused in Wasm but must be defined to satisfy the linker
size_t APIClient::WriteCallback(void*, size_t size, size_t nmemb, void*) {
    return size * nmemb;
}

// ---------------------------------------------------------------------------
// Auth
// ---------------------------------------------------------------------------
bool APIClient::login(const std::string& username, const std::string& password) {
    json data;
    data["username"] = username;
    data["password"] = password;

    std::string response = performRequest("/auth/login", "POST", data.dump(), false);
    try {
        json r = json::parse(response);
        if (r.contains("token")) {
            jwtToken_  = r["token"];
            playerId_  = r.value("player_id", -1);
            std::cout << "[APIClient WASM] Logged in." << std::endl;
            return true;
        }
    } catch (...) {}
    std::cerr << "[APIClient WASM] Login failed." << std::endl;
    return false;
}

bool APIClient::registerUser(const std::string& username,
                              const std::string& password,
                              const std::string& email)
{
    json data;
    data["username"] = username;
    data["password"] = password;
    if (!email.empty()) data["email"] = email;

    std::string response = performRequest("/auth/register", "POST", data.dump(), false);
    try {
        json r = json::parse(response);
        if (r.contains("token")) {
            jwtToken_ = r["token"];
            playerId_ = r.value("player_id", -1);
            return true;
        }
    } catch (...) {}
    std::cerr << "[APIClient WASM] Register failed." << std::endl;
    return false;
}

void APIClient::logout() {
    jwtToken_.clear();
    playerId_ = -1;
}

// ---------------------------------------------------------------------------
// Player
// ---------------------------------------------------------------------------
int APIClient::createPlayer(const std::string& username, const std::string& email) {
    json data;
    data["username"]    = username;
    data["email"]       = email;
    data["total_score"] = 0;
    data["games_played"]= 0;
    data["highest_level"]= 1;

    std::string response = performRequest("/game/players", "POST", data.dump());
    try {
        json r = json::parse(response);
        return r.value("id", -1);
    } catch (...) {}
    return -1;
}

// ---------------------------------------------------------------------------
// Game sessions
// ---------------------------------------------------------------------------
int APIClient::startGameSession(int playerId, int worldId) {
    json data;
    data["player_id"] = playerId;
    data["world_id"]  = worldId;
    data["score"]     = 0;

    std::string response = performRequest("/game/sessions", "POST", data.dump());
    try {
        json r = json::parse(response);
        int id = r.value("id", -1);
        std::cout << "[APIClient WASM] Session started: " << id << std::endl;
        return id;
    } catch (...) {}
    return -1;
}

bool APIClient::updateGameSession(int sessionId, int score, int level,
                                   int enemiesKilled, int duration)
{
    json data;
    data["score"]           = score;
    data["level_reached"]   = level;
    data["enemies_killed"]  = enemiesKilled;
    data["duration_seconds"]= duration;

    std::string endpoint = "/game/sessions/" + std::to_string(sessionId);
    std::string response = performRequest(endpoint, "PATCH", data.dump());
    return !response.empty();
}

bool APIClient::endGameSession(int sessionId, int finalScore, int level,
                                bool completed)
{
    json data;
    data["score"]        = finalScore;
    data["level_reached"]= level;
    data["completed"]    = completed;

    std::string endpoint = "/game/sessions/" + std::to_string(sessionId);
    std::string response = performRequest(endpoint, "PATCH", data.dump());
    return !response.empty();
}

// ---------------------------------------------------------------------------
// Public endpoints
// ---------------------------------------------------------------------------
json APIClient::getLeaderboard(int limit) {
    std::string endpoint = "/game/leaderboard?limit=" + std::to_string(limit);
    std::string response = performRequest(endpoint, "GET", "", false);
    try { return json::parse(response); } catch (...) {}
    return json::array();
}

json APIClient::getStats() {
    std::string response = performRequest("/game/stats", "GET", "", false);
    try { return json::parse(response); } catch (...) {}
    return json::object();
}

} // namespace SI3LN

#endif // __EMSCRIPTEN__
