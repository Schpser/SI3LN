#ifndef SPECIAL_ATTACK_HPP
#define SPECIAL_ATTACK_HPP

#include "Entity.hpp"

enum class SpecialAttackType {
    LASER_BEAM,      // Continuous beam across screen
    BOMB_BLAST,      // Area damage explosion
    WAVE_ATTACK,     // Expanding circular wave
    MISSILE_BARRAGE  // Multiple homing missiles
};

class SpecialAttack : public Entity {
private:
    SpecialAttackType attackType;
    int damage;
    float range;
    unsigned int startTime;
    unsigned int duration;
    bool hitEnemies;
    
    // Animation/effect data
    float currentRadius;  // For wave attacks
    float expansionSpeed;
    
public:
    SpecialAttack(float x, float y, SpecialAttackType type);
    ~SpecialAttack() override = default;
    
    void update(float deltaTime) override;
    void render() override;
    
    // Combat
    int getDamage() const { return damage; }
    float getRange() const { return range; }
    bool hasHitEnemies() const { return hitEnemies; }
    void markEnemiesHit() { hitEnemies = true; }
    
    // Getters
    SpecialAttackType getAttackType() const { return attackType; }
    bool isFinished(unsigned int currentTime) const;
    float getCurrentRadius() const { return currentRadius; }
    
    // Check if point is within attack range
    bool isInRange(const Vector2D& point) const;
};

#endif // SPECIAL_ATTACK_HPP
