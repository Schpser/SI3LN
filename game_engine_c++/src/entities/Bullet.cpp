#include "../include/Bullet.hpp"
#include <iostream>

Bullet::Bullet(float x, float y, bool isPlayerBullet, int damage)
    : Entity(x, y, 
             isPlayerBullet ? Constants::BULLET_WIDTH : Constants::ENEMY_BULLET_WIDTH,
             isPlayerBullet ? Constants::BULLET_HEIGHT : Constants::ENEMY_BULLET_HEIGHT,
             isPlayerBullet ? EntityType::BULLET_PLAYER : EntityType::BULLET_ENEMY),
      damage(damage), isPlayerBullet(isPlayerBullet),
      speed(isPlayerBullet ? Constants::BULLET_SPEED : Constants::ENEMY_BULLET_SPEED) {
    
    // Set velocity based on direction
    if (isPlayerBullet) {
        velocity = Vector2D(0, -speed); // Move up
        textureId = "bullet_player";
    } else {
        velocity = Vector2D(0, speed); // Move down
        textureId = "bullet_enemy";
    }
}

void Bullet::update(float deltaTime) {
    if (!active) return;
    
    position += velocity * deltaTime;
}

void Bullet::render() {
    if (!active) return;
    
    std::cout << "Rendering bullet at (" << position.x << ", " << position.y << ")" << std::endl;
}

bool Bullet::shouldRemove(int screenHeight) const {
    if (!active) return true;
    
    if (isPlayerBullet) {
        return position.y < -height;
    } else {
        return position.y > screenHeight + height;
    }
}
