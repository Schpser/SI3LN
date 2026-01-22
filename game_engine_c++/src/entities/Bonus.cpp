#include "../include/Bonus.hpp"
#include <iostream>
#include <ctime>

Bonus::Bonus(float x, float y, BonusType type)
    : Entity(x, y, Constants::BONUS_WIDTH, Constants::BONUS_HEIGHT, EntityType::BONUS),
      bonusType(type), fallSpeed(Constants::BONUS_FALL_SPEED),
      spawnTime(static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC)),
      lifetime(10000) { // 10 seconds
    
    velocity = Vector2D(0, fallSpeed);
    
    // Set texture based on bonus type
    switch (type) {
        case BonusType::SHIELD:
            textureId = "bonus_shield";
            break;
        case BonusType::MEGA_SHOT:
            textureId = "bonus_mega_shot";
            break;
        case BonusType::EXTRA_LIFE:
            textureId = "bonus_extra_life";
            break;
        case BonusType::FREEZE_ENEMIES:
            textureId = "bonus_freeze";
            break;
        case BonusType::SPEED_BOOST:
            textureId = "bonus_speed";
            break;
    }
}

void Bonus::update(float deltaTime) {
    if (!active) return;
    
    position += velocity * deltaTime;
    
    unsigned int currentTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    if (hasExpired(currentTime)) {
        active = false;
    }
}

void Bonus::render() {
    if (!active) return;
    
    std::cout << "Rendering bonus at (" << position.x << ", " << position.y << ")" << std::endl;
}

bool Bonus::hasExpired(unsigned int currentTime) const {
    return (currentTime - spawnTime) >= lifetime;
}

bool Bonus::isOffScreen(int screenHeight) const {
    return position.y > screenHeight + height;
}

std::string Bonus::getDescription() const {
    switch (bonusType) {
        case BonusType::SHIELD:
            return "Shield - Protects from one hit";
        case BonusType::MEGA_SHOT:
            return "Mega Shot - Powerful bullets for 5 seconds";
        case BonusType::EXTRA_LIFE:
            return "Extra Life - Gain one life";
        case BonusType::FREEZE_ENEMIES:
            return "Freeze - Freeze all enemies briefly";
        case BonusType::SPEED_BOOST:
            return "Speed Boost - Move faster temporarily";
        default:
            return "Unknown Bonus";
    }
}
