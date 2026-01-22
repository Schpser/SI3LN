#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Vector2D.hpp"
#include "Constants.hpp"
#include <string>

class Entity {
protected:
    Vector2D position;
    Vector2D velocity;
    float width;
    float height;
    bool active;
    EntityType type;
    std::string textureId;
    
public:
    Entity(float x, float y, float w, float h, EntityType type);
    virtual ~Entity() = default;
    
    // Core methods
    virtual void update(float deltaTime) = 0;
    virtual void render() = 0;
    
    // Getters
    Vector2D getPosition() const { return position; }
    Vector2D getVelocity() const { return velocity; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
    bool isActive() const { return active; }
    EntityType getType() const { return type; }
    std::string getTextureId() const { return textureId; }
    
    // Setters
    void setPosition(const Vector2D& pos) { position = pos; }
    void setVelocity(const Vector2D& vel) { velocity = vel; }
    void setActive(bool active) { this->active = active; }
    void setTextureId(const std::string& id) { textureId = id; }
    
    // Collision detection
    bool collidesWith(const Entity& other) const;
    bool isOutOfBounds(int screenWidth, int screenHeight) const;
    
    // Movement
    void move(float dx, float dy);
    void clampToScreen(int screenWidth, int screenHeight);
};

#endif // ENTITY_HPP
