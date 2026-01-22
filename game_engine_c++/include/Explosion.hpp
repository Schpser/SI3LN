#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "Vector2D.hpp"
#include <string>

class Explosion {
private:
    Vector2D position;
    float width;
    float height;
    unsigned int startTime;
    unsigned int duration;
    bool active;
    std::string textureId;
    float scale;
    
public:
    Explosion(float x, float y, unsigned int duration = 500);
    ~Explosion() = default;
    
    void update(unsigned int currentTime);
    void render();
    
    // Getters
    bool isActive() const { return active; }
    Vector2D getPosition() const { return position; }
    float getScale() const { return scale; }
    std::string getTextureId() const { return textureId; }
    
    // Setters
    void setTextureId(const std::string& id) { textureId = id; }
};

#endif // EXPLOSION_HPP
