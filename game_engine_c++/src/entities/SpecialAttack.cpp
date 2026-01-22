#include "../include/SpecialAttack.hpp"
#include <iostream>
#include <ctime>
#include <cmath>

SpecialAttack::SpecialAttack(float x, float y, SpecialAttackType type)
    : Entity(x, y, Constants::SPECIAL_ATTACK_WIDTH, Constants::SPECIAL_ATTACK_HEIGHT, EntityType::SPECIAL_ATTACK),
      attackType(type), damage(5), range(200.0f),
      startTime(static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC)),
      duration(2000), hitEnemies(false), currentRadius(0), expansionSpeed(150.0f) {
    
    switch (type) {
        case SpecialAttackType::LASER_BEAM:
            damage = 3;
            range = 1000.0f;
            duration = 1000;
            textureId = "special_laser";
            break;
            
        case SpecialAttackType::BOMB_BLAST:
            damage = 10;
            range = 150.0f;
            duration = 500;
            textureId = "special_bomb";
            break;
            
        case SpecialAttackType::WAVE_ATTACK:
            damage = 4;
            range = 300.0f;
            duration = 2000;
            expansionSpeed = 200.0f;
            textureId = "special_wave";
            break;
            
        case SpecialAttackType::MISSILE_BARRAGE:
            damage = 6;
            range = 250.0f;
            duration = 1500;
            textureId = "special_missile";
            break;
    }
}

void SpecialAttack::update(float deltaTime) {
    if (!active) return;
    
    unsigned int currentTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    
    if (isFinished(currentTime)) {
        active = false;
        return;
    }
    
    // Update based on attack type
    if (attackType == SpecialAttackType::WAVE_ATTACK) {
        currentRadius += expansionSpeed * deltaTime;
        if (currentRadius > range) {
            currentRadius = range;
        }
    }
}

void SpecialAttack::render() {
    if (!active) return;
    
    std::cout << "Rendering special attack at (" << position.x << ", " << position.y << ")" << std::endl;
}

bool SpecialAttack::isFinished(unsigned int currentTime) const {
    return (currentTime - startTime) >= duration;
}

bool SpecialAttack::isInRange(const Vector2D& point) const {
    switch (attackType) {
        case SpecialAttackType::LASER_BEAM:
            // Check if point is in the beam path (vertical line)
            return std::abs(point.x - position.x) < width / 2.0f && point.y < position.y;
            
        case SpecialAttackType::BOMB_BLAST:
        case SpecialAttackType::MISSILE_BARRAGE:
            // Check if point is within circular range
            return position.distance(point) <= range;
            
        case SpecialAttackType::WAVE_ATTACK:
            // Check if point is within the expanding wave
            {
                float dist = position.distance(point);
                float waveThickness = 30.0f;
                return dist <= currentRadius && dist >= (currentRadius - waveThickness);
            }
            
        default:
            return false;
    }
}
