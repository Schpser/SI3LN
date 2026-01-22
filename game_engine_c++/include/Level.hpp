#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "World.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Bonus.hpp"
#include "Explosion.hpp"
#include "SpecialAttack.hpp"
#include <vector>
#include <memory>

class Level {
private:
    int levelNumber;
    World* world;
    bool completed;
    bool hasBoss;
    
    // Entities
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Bullet>> playerBullets;
    std::vector<std::shared_ptr<Bullet>> enemyBullets;
    std::vector<std::shared_ptr<Bonus>> bonuses;
    std::vector<std::shared_ptr<Explosion>> explosions;
    std::vector<std::shared_ptr<SpecialAttack>> specialAttacks;
    
    // Level state
    int enemiesKilled;
    int totalEnemies;
    unsigned int startTime;
    unsigned int elapsedTime;
    
    // Wave system
    int currentWave;
    int totalWaves;
    bool waveActive;
    unsigned int waveStartTime;
    unsigned int waveCooldown;
    
public:
    Level(int levelNumber, World* world);
    ~Level() = default;
    
    // Level lifecycle
    void initialize();
    void start();
    void update(float deltaTime, Player& player);
    void render();
    void reset();
    
    // Enemy management
    void spawnEnemies(int count);
    void spawnBoss();
    void spawnWave();
    void removeDeadEnemies();
    
    // Combat
    void handleCollisions(Player& player);
    void playerShoot(const Vector2D& playerPos);
    void enemyShoot(const Vector2D& enemyPos);
    void createExplosion(const Vector2D& pos);
    void createSpecialAttack(const Vector2D& pos, SpecialAttackType type);
    
    // Bonus management
    void spawnBonus(const Vector2D& pos, BonusType type);
    void checkBonusCollision(Player& player);
    void applyBonus(Player& player, BonusType type);
    
    // Level progression
    bool isCompleted() const { return completed; }
    void checkCompletion();
    int getRemainingEnemies() const;
    
    // Getters
    int getLevelNumber() const { return levelNumber; }
    World* getWorld() const { return world; }
    int getEnemiesKilled() const { return enemiesKilled; }
    int getTotalEnemies() const { return totalEnemies; }
    int getCurrentWave() const { return currentWave; }
    int getTotalWaves() const { return totalWaves; }
    unsigned int getElapsedTime() const { return elapsedTime; }
    
    // Entity getters
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return enemies; }
    const std::vector<std::shared_ptr<Bullet>>& getPlayerBullets() const { return playerBullets; }
    const std::vector<std::shared_ptr<Bullet>>& getEnemyBullets() const { return enemyBullets; }
    const std::vector<std::shared_ptr<Bonus>>& getBonuses() const { return bonuses; }
    const std::vector<std::shared_ptr<Explosion>>& getExplosions() const { return explosions; }
};

#endif // LEVEL_HPP
