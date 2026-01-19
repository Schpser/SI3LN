#pragma once

#include "Vector2D.h"
#include <SDL2/SDL.h>
#include <memory>

namespace SI3LN {

class Entity {
public:
    Entity(float x, float y, int width, int height)
        : position(x, y), velocity(0.0f, 0.0f)
        , width(width), height(height), alive(true) {}
    
    virtual ~Entity() = default;
    
    // Pure virtual methods that must be implemented by derived classes
    virtual void update(float deltaTime) = 0;
    virtual void render(SDL_Renderer* renderer) = 0;
    
    // Getters
    Vector2D getPosition() const { return position; }
    Vector2D getVelocity() const { return velocity; }
    SDL_Rect getRect() const {
        return {
            static_cast<int>(position.x - width / 2),
            static_cast<int>(position.y - height / 2),
            width,
            height
        };
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isAlive() const { return alive; }
    
    // Setters
    void setPosition(const Vector2D& pos) { position = pos; }
    void setVelocity(const Vector2D& vel) { velocity = vel; }
    void kill() { alive = false; }
    
    // Collision detection
    bool intersects(const Entity& other) const {
        SDL_Rect thisRect = getRect();
        SDL_Rect otherRect = other.getRect();
        return SDL_HasIntersection(&thisRect, &otherRect);
    }
    
    float distanceTo(const Entity& other) const {
        return position.distance(other.position);
    }

protected:
    Vector2D position;
    Vector2D velocity;
    int width;
    int height;
    bool alive;
};

using EntityPtr = std::shared_ptr<Entity>;

} // namespace SI3LN
