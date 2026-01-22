#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"
#include "Constants.hpp"

class Enemy : public Entity {
private:
    EnemyType enemyType;
    int health;
    int maxHealth;
    float speed;
    unsigned int lastShotTime;
    unsigned int shootCooldown;
    MovementPattern movementPattern;
    
    // Movement pattern specific data
    Vector2D patternStartPos;
    float patternTime;
    float patternRadius;
    Vector2D direction;
    
    // AI behavior
    float aggroRange;
    bool isAggro;
    
public:
    Enemy(float x, float y, EnemyType type, int level = 1);
    ~Enemy() override = default;
    
    void update(float deltaTime) override;
    void render() override;
    
    // Combat
    bool canShoot() const;
    void shoot();
    void takeDamage(int damage);
    bool isDead() const { return health <= 0; }
    
    // Movement
    void updateMovement(float deltaTime);
    void setMovementPattern(MovementPattern pattern);
    void reverseDirection();
    
    // AI
    void updateAI(const Vector2D& playerPos, float deltaTime);
    void checkAggro(const Vector2D& playerPos);
    
    // Getters
    EnemyType getEnemyType() const { return enemyType; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    MovementPattern getMovementPattern() const { return movementPattern; }
    
    // Score value based on enemy type
    int getScoreValue() const;
    
    // Bonus drop chance
    bool shouldDropBonus() const;
    BonusType getRandomBonusType() const;
};

#endif // ENEMY_HPP
