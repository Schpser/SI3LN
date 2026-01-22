#include "../include/Entity.hpp"
#include <algorithm>

Entity::Entity(float x, float y, float w, float h, EntityType type)
    : position(x, y), velocity(0, 0), width(w), height(h), 
      active(true), type(type), textureId("") {
}

bool Entity::collidesWith(const Entity& other) const {
    if (!active || !other.active) return false;
    
    float left1 = position.x - width / 2.0f;
    float right1 = position.x + width / 2.0f;
    float top1 = position.y - height / 2.0f;
    float bottom1 = position.y + height / 2.0f;
    
    float left2 = other.position.x - other.width / 2.0f;
    float right2 = other.position.x + other.width / 2.0f;
    float top2 = other.position.y - other.height / 2.0f;
    float bottom2 = other.position.y + other.height / 2.0f;
    
    return !(right1 < left2 || left1 > right2 || 
             bottom1 < top2 || top1 > bottom2);
}

bool Entity::isOutOfBounds(int screenWidth, int screenHeight) const {
    return position.x < -width || position.x > screenWidth + width ||
           position.y < -height || position.y > screenHeight + height;
}

void Entity::move(float dx, float dy) {
    position.x += dx;
    position.y += dy;
}

void Entity::clampToScreen(int screenWidth, int screenHeight) {
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    
    position.x = std::max(halfWidth, std::min(position.x, static_cast<float>(screenWidth) - halfWidth));
    position.y = std::max(halfHeight, std::min(position.y, static_cast<float>(screenHeight) - halfHeight));
}
