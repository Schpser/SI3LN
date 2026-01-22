#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <random>

namespace Utils {
    // Random number generation
    int randomInt(int min, int max);
    float randomFloat(float min, float max);
    bool randomChance(int percentage);
    
    // String utilities
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
    
    // Math utilities
    float clamp(float value, float min, float max);
    int clamp(int value, int min, int max);
    float lerp(float a, float b, float t);
    float distance(float x1, float y1, float x2, float y2);
    
    // File utilities
    bool fileExists(const std::string& filename);
    std::string readFile(const std::string& filename);
    bool writeFile(const std::string& filename, const std::string& content);
    std::vector<std::string> listFiles(const std::string& directory, const std::string& extension = "");
    
    // Timing utilities
    unsigned int getTimeMs();
    void sleep(unsigned int milliseconds);
    
    // Color utilities (for converting hex to RGB)
    struct Color {
        unsigned char r, g, b, a;
        Color() : r(0), g(0), b(0), a(255) {}
        Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
            : r(r), g(g), b(b), a(a) {}
    };
    
    Color hexToColor(const std::string& hex);
    std::string colorToHex(const Color& color);
}

#endif // UTILS_HPP
