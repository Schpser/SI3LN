#include "../include/Enemy.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

Enemy::Enemy(float x, float y, EnemyType type, int level)
    : Entity(x, y, Constants::ENEMY_WIDTH, Constants::ENEMY_HEIGHT, EntityType::ENEMY),
      enemyType(type), health(1), maxHealth(1), speed(Constants::ENEMY_SPEED),
      lastShotTime(0), shootCooldown(Constants::ENEMY_SHOOT_COOLDOWN),
      movementPattern(MovementPattern::HORIZONTAL),
      patternStartPos(x, y), patternTime(0), patternRadius(50.0f),
      direction(1, 0), aggroRange(300.0f), isAggro(false) {
    
    // Set properties based on enemy type
    switch (type) {
        case EnemyType::BASIC:
            health = 1;
            speed = Constants::ENEMY_SPEED;
            shootCooldown = 3000;
            movementPattern = MovementPattern::HORIZONTAL;
            break;
            
        case EnemyType::FAST:
            health = 1;
            speed = Constants::ENEMY_SPEED * 1.5f;
            shootCooldown = 2000;
            movementPattern = MovementPattern::ZIGZAG;
            break;
            
        case EnemyType::TANK:
            health = 3;
            speed = Constants::ENEMY_SPEED * 0.7f;
            shootCooldown = 4000;
            movementPattern = MovementPattern::VERTICAL;
            width = Constants::ENEMY_WIDTH * 1.2f;
            height = Constants::ENEMY_HEIGHT * 1.2f;
            break;
            
        case EnemyType::SHOOTER:
            health = 2;
            speed = Constants::ENEMY_SPEED;
            shootCooldown = 1500;
            movementPattern = MovementPattern::CIRCULAR;
            aggroRange = 400.0f;
            break;
            
        case EnemyType::BOSS:
            health = Constants::BOSS_HEALTH;
            speed = Constants::ENEMY_SPEED * 0.5f;
            shootCooldown = 1000;
            movementPattern = MovementPattern::DIAGONAL;
            width = Constants::BOSS_WIDTH;
            height = Constants::BOSS_HEIGHT;
            aggroRange = 500.0f;
            break;
    }
    
    maxHealth = health;
    
    // Apply level scaling
    health = static_cast<int>(health * (1.0f + (level - 1) * 0.2f));
    maxHealth = health;
    
    textureId = "enemy_" + std::to_string(static_cast<int>(type));
}

void Enemy::update(float deltaTime) {
    if (!active) return;
    
    updateMovement(deltaTime);
}

void Enemy::render() {
    if (!active) return;
    
    std::cout << "Rendering enemy at (" << position.x << ", " << position.y << ")" << std::endl;
}

bool Enemy::canShoot() const {
    if (!active) return false;
    
    unsigned int currentTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    return (currentTime - lastShotTime) >= shootCooldown;
}

void Enemy::shoot() {
    if (canShoot()) {
        lastShotTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    }
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        active = false;
    }
}

void Enemy::updateMovement(float deltaTime) {
    patternTime += deltaTime;
    
    switch (movementPattern) {
        case MovementPattern::HORIZONTAL:
            velocity.x = direction.x * speed;
            velocity.y = 0;
            position += velocity * deltaTime;
            break;
            
        case MovementPattern::VERTICAL:
            velocity.x = 0;
            velocity.y = direction.y * speed;
            position += velocity * deltaTime;
            break;
            
        case MovementPattern::DIAGONAL:
            velocity = direction * speed;
            position += velocity * deltaTime;
            break;
            
        case MovementPattern::CIRCULAR:
            {
                float angle = patternTime * 2.0f;
                position.x = patternStartPos.x + patternRadius * std::cos(angle);
                position.y = patternStartPos.y + patternRadius * std::sin(angle);
            }
            break;
            
        case MovementPattern::ZIGZAG:
            {
                velocity.x = direction.x * speed;
                velocity.y = std::sin(patternTime * 5.0f) * speed * 0.5f;
                position += velocity * deltaTime;
            }
            break;
            
        case MovementPattern::RANDOM:
            if (static_cast<int>(patternTime * 100) % 200 == 0) {
                direction.x = (rand() % 3 - 1) * 1.0f;
                direction.y = (rand() % 3 - 1) * 1.0f;
                if (direction.length() > 0) {
                    direction = direction.normalized();
                }
            }
            velocity = direction * speed;
            position += velocity * deltaTime;
            break;
    }
}

void Enemy::setMovementPattern(MovementPattern pattern) {
    movementPattern = pattern;
    patternStartPos = position;
    patternTime = 0;
}

void Enemy::reverseDirection() {
    direction.x = -direction.x;
    direction.y = -direction.y;
}

void Enemy::updateAI(const Vector2D& playerPos, float deltaTime) {
    checkAggro(playerPos);
    
    if (isAggro && enemyType == EnemyType::SHOOTER) {
        // Move towards player
        Vector2D toPlayer = playerPos - position;
        if (toPlayer.length() > aggroRange * 0.5f) {
            direction = toPlayer.normalized();
        }
    }
}

void Enemy::checkAggro(const Vector2D& playerPos) {
    float dist = position.distance(playerPos);
    isAggro = (dist <= aggroRange);
}

int Enemy::getScoreValue() const {
    switch (enemyType) {
        case EnemyType::BASIC:
            return Constants::SCORE_ENEMY;
        case EnemyType::FAST:
            return Constants::SCORE_ENEMY * 1.5;
        case EnemyType::TANK:
            return Constants::SCORE_ENEMY * 2;
        case EnemyType::SHOOTER:
            return Constants::SCORE_ENEMY * 1.8;
        case EnemyType::BOSS:
            return Constants::SCORE_BOSS;
        default:
            return Constants::SCORE_ENEMY;
    }
}

bool Enemy::shouldDropBonus() const {
    int chance = rand() % 100;
    return chance < 15; // 15% chance to drop a bonus
}

BonusType Enemy::getRandomBonusType() const {
    int roll = rand() % 100;
    
    if (roll < Constants::BONUS_EXTRA_LIFE_CHANCE) {
        return BonusType::EXTRA_LIFE;
    } else if (roll < Constants::BONUS_EXTRA_LIFE_CHANCE + Constants::BONUS_SHIELD_CHANCE) {
        return BonusType::SHIELD;
    } else if (roll < Constants::BONUS_EXTRA_LIFE_CHANCE + Constants::BONUS_SHIELD_CHANCE + Constants::BONUS_MEGA_SHOT_CHANCE) {
        return BonusType::MEGA_SHOT;
    } else {
        return BonusType::FREEZE_ENEMIES;
    }
}
