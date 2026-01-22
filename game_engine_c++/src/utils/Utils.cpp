#include "../include/Utils.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <dirent.h>
#include <sys/stat.h>

namespace Utils {
    // Random number generation
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    int randomInt(int min, int max) {
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }
    
    float randomFloat(float min, float max) {
        std::uniform_real_distribution<> dis(min, max);
        return static_cast<float>(dis(gen));
    }
    
    bool randomChance(int percentage) {
        return randomInt(0, 99) < percentage;
    }
    
    // String utilities
    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        
        if (start == std::string::npos) return "";
        return str.substr(start, end - start + 1);
    }
    
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }
    
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }
    
    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        return result;
    }
    
    // Math utilities
    float clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
    
    int clamp(int value, int min, int max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }
    
    float lerp(float a, float b, float t) {
        return a + (b - a) * clamp(t, 0.0f, 1.0f);
    }
    
    float distance(float x1, float y1, float x2, float y2) {
        float dx = x2 - x1;
        float dy = y2 - y1;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    // File utilities
    bool fileExists(const std::string& filename) {
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    }
    
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return "";
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    bool writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        return true;
    }
    
    std::vector<std::string> listFiles(const std::string& directory, const std::string& extension) {
        std::vector<std::string> files;
        DIR* dir = opendir(directory.c_str());
        
        if (dir == nullptr) {
            return files;
        }
        
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename = entry->d_name;
            
            if (filename == "." || filename == "..") {
                continue;
            }
            
            if (extension.empty() || filename.find(extension) != std::string::npos) {
                files.push_back(filename);
            }
        }
        
        closedir(dir);
        return files;
    }
    
    // Timing utilities
    unsigned int getTimeMs() {
        return static_cast<unsigned int>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count()
        );
    }
    
    void sleep(unsigned int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
    
    // Color utilities
    Color hexToColor(const std::string& hex) {
        Color color;
        std::string h = hex;
        
        // Remove '#' if present
        if (h[0] == '#') {
            h = h.substr(1);
        }
        
        if (h.length() == 6) {
            color.r = std::stoi(h.substr(0, 2), nullptr, 16);
            color.g = std::stoi(h.substr(2, 2), nullptr, 16);
            color.b = std::stoi(h.substr(4, 2), nullptr, 16);
            color.a = 255;
        } else if (h.length() == 8) {
            color.r = std::stoi(h.substr(0, 2), nullptr, 16);
            color.g = std::stoi(h.substr(2, 2), nullptr, 16);
            color.b = std::stoi(h.substr(4, 2), nullptr, 16);
            color.a = std::stoi(h.substr(6, 2), nullptr, 16);
        }
        
        return color;
    }
    
    std::string colorToHex(const Color& color) {
        char hex[10];
        snprintf(hex, sizeof(hex), "#%02X%02X%02X%02X", color.r, color.g, color.b, color.a);
        return std::string(hex);
    }
}
