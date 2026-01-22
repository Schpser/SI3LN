#include "../include/Level.hpp"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>

Level::Level(int levelNumber, World* world)
    : levelNumber(levelNumber), world(world), completed(false),
      hasBoss(world->hasBoss(levelNumber)),
      enemiesKilled(0), totalEnemies(0),
      startTime(0), elapsedTime(0),
      currentWave(1), totalWaves(3), waveActive(false),
      waveStartTime(0), waveCooldown(5000) {
}

void Level::initialize() {
    // Clear all entities
    enemies.clear();
    playerBullets.clear();
    enemyBullets.clear();
    bonuses.clear();
    explosions.clear();
    specialAttacks.clear();
    
    enemiesKilled = 0;
    completed = false;
    currentWave = 1;
    waveActive = false;
    
    totalEnemies = world->getEnemyCount(levelNumber);
    
    std::cout << "Level " << levelNumber << " initialized. Total enemies: " << totalEnemies << std::endl;
}

void Level::start() {
    startTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    spawnWave();
}

void Level::update(float deltaTime, Player& player) {
    unsigned int currentTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    elapsedTime = currentTime - startTime;
    
    // Update all enemies
    for (auto& enemy : enemies) {
        if (enemy->isActive()) {
            enemy->update(deltaTime);
            enemy->updateAI(player.getPosition(), deltaTime);
            
            // Enemy shooting
            if (enemy->canShoot() && rand() % 100 < 5) { // 5% chance per frame
                enemyShoot(enemy->getPosition());
            }
        }
    }
    
    // Update all bullets
    for (auto& bullet : playerBullets) {
        bullet->update(deltaTime);
    }
    
    for (auto& bullet : enemyBullets) {
        bullet->update(deltaTime);
    }
    
    // Update bonuses
    for (auto& bonus : bonuses) {
        bonus->update(deltaTime);
    }
    
    // Update explosions
    for (auto& explosion : explosions) {
        explosion->update(currentTime);
    }
    
    // Update special attacks
    for (auto& attack : specialAttacks) {
        attack->update(deltaTime);
    }
    
    // Handle collisions
    handleCollisions(player);
    
    // Remove dead/inactive entities
    removeDeadEnemies();
    
    // Remove out-of-bounds bullets
    playerBullets.erase(
        std::remove_if(playerBullets.begin(), playerBullets.end(),
            [](const std::shared_ptr<Bullet>& b) { return !b->isActive() || b->shouldRemove(768); }),
        playerBullets.end()
    );
    
    enemyBullets.erase(
        std::remove_if(enemyBullets.begin(), enemyBullets.end(),
            [](const std::shared_ptr<Bullet>& b) { return !b->isActive() || b->shouldRemove(768); }),
        enemyBullets.end()
    );
    
    // Remove expired bonuses
    bonuses.erase(
        std::remove_if(bonuses.begin(), bonuses.end(),
            [currentTime](const std::shared_ptr<Bonus>& b) { 
                return !b->isActive() || b->hasExpired(currentTime) || b->isOffScreen(768); 
            }),
        bonuses.end()
    );
    
    // Remove finished explosions
    explosions.erase(
        std::remove_if(explosions.begin(), explosions.end(),
            [](const std::shared_ptr<Explosion>& e) { return !e->isActive(); }),
        explosions.end()
    );
    
    // Remove finished special attacks
    specialAttacks.erase(
        std::remove_if(specialAttacks.begin(), specialAttacks.end(),
            [](const std::shared_ptr<SpecialAttack>& s) { return !s->isActive(); }),
        specialAttacks.end()
    );
    
    // Check for wave spawning
    if (!waveActive && currentWave < totalWaves && enemies.empty()) {
        if (currentTime - waveStartTime >= waveCooldown) {
            currentWave++;
            spawnWave();
        }
    }
    
    // Check level completion
    checkCompletion();
}

void Level::render() {
    // Render all entities
    for (auto& enemy : enemies) {
        if (enemy->isActive()) {
            enemy->render();
        }
    }
    
    for (auto& bullet : playerBullets) {
        bullet->render();
    }
    
    for (auto& bullet : enemyBullets) {
        bullet->render();
    }
    
    for (auto& bonus : bonuses) {
        bonus->render();
    }
    
    for (auto& explosion : explosions) {
        explosion->render();
    }
    
    for (auto& attack : specialAttacks) {
        attack->render();
    }
}

void Level::reset() {
    initialize();
}

void Level::spawnEnemies(int count) {
    int screenWidth = 1024; // TODO: Get from game manager
    int rows = 3;
    int cols = count / rows;
    int spacing = 80;
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            float x = 100 + col * spacing;
            float y = 100 + row * spacing;
            
            EnemyType type = EnemyType::BASIC;
            if (rand() % 100 < 20) type = EnemyType::FAST;
            if (rand() % 100 < 15) type = EnemyType::SHOOTER;
            if (rand() % 100 < 10) type = EnemyType::TANK;
            
            enemies.push_back(std::make_shared<Enemy>(x, y, type, levelNumber));
        }
    }
    
    std::cout << "Spawned " << enemies.size() << " enemies" << std::endl;
}

void Level::spawnBoss() {
    int screenWidth = 1024;
    float x = screenWidth / 2.0f;
    float y = 150;
    
    enemies.push_back(std::make_shared<Enemy>(x, y, EnemyType::BOSS, levelNumber));
    std::cout << "Boss spawned!" << std::endl;
}

void Level::spawnWave() {
    waveActive = true;
    waveStartTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    
    int enemiesInWave = totalEnemies / totalWaves;
    spawnEnemies(enemiesInWave);
    
    if (hasBoss && currentWave == totalWaves) {
        spawnBoss();
    }
}

void Level::removeDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const std::shared_ptr<Enemy>& e) { return !e->isActive(); }),
        enemies.end()
    );
}

void Level::handleCollisions(Player& player) {
    // Player bullets vs enemies
    for (auto& bullet : playerBullets) {
        if (!bullet->isActive()) continue;
        
        for (auto& enemy : enemies) {
            if (!enemy->isActive()) continue;
            
            if (bullet->collidesWith(*enemy)) {
                enemy->takeDamage(bullet->getDamage());
                bullet->setActive(false);
                
                createExplosion(enemy->getPosition());
                
                if (enemy->isDead()) {
                    player.addScore(enemy->getScoreValue());
                    enemiesKilled++;
                    
                    // Drop bonus chance
                    if (enemy->shouldDropBonus()) {
                        spawnBonus(enemy->getPosition(), enemy->getRandomBonusType());
                    }
                }
                break;
            }
        }
    }
    
    // Enemy bullets vs player
    for (auto& bullet : enemyBullets) {
        if (!bullet->isActive()) continue;
        
        if (bullet->collidesWith(player)) {
            player.loseLife();
            bullet->setActive(false);
            createExplosion(player.getPosition());
        }
    }
    
    // Enemies vs player
    for (auto& enemy : enemies) {
        if (!enemy->isActive()) continue;
        
        if (enemy->collidesWith(player)) {
            player.loseLife();
            enemy->takeDamage(1);
            createExplosion(player.getPosition());
        }
    }
    
    // Bonuses vs player
    checkBonusCollision(player);
    
    // Special attacks vs enemies
    for (auto& attack : specialAttacks) {
        if (!attack->isActive() || attack->hasHitEnemies()) continue;
        
        for (auto& enemy : enemies) {
            if (!enemy->isActive()) continue;
            
            if (attack->isInRange(enemy->getPosition())) {
                enemy->takeDamage(attack->getDamage());
                createExplosion(enemy->getPosition());
                
                if (enemy->isDead()) {
                    player.addScore(enemy->getScoreValue());
                    enemiesKilled++;
                }
            }
        }
        attack->markEnemiesHit();
    }
}

void Level::playerShoot(const Vector2D& playerPos) {
    int damage = 1;
    playerBullets.push_back(std::make_shared<Bullet>(playerPos.x, playerPos.y, true, damage));
}

void Level::enemyShoot(const Vector2D& enemyPos) {
    enemyBullets.push_back(std::make_shared<Bullet>(enemyPos.x, enemyPos.y, false, 1));
}

void Level::createExplosion(const Vector2D& pos) {
    explosions.push_back(std::make_shared<Explosion>(pos.x, pos.y, 500));
}

void Level::createSpecialAttack(const Vector2D& pos, SpecialAttackType type) {
    specialAttacks.push_back(std::make_shared<SpecialAttack>(pos.x, pos.y, type));
}

void Level::spawnBonus(const Vector2D& pos, BonusType type) {
    bonuses.push_back(std::make_shared<Bonus>(pos.x, pos.y, type));
}

void Level::checkBonusCollision(Player& player) {
    for (auto& bonus : bonuses) {
        if (!bonus->isActive()) continue;
        
        if (bonus->collidesWith(player)) {
            applyBonus(player, bonus->getBonusType());
            bonus->setActive(false);
        }
    }
}

void Level::applyBonus(Player& player, BonusType type) {
    switch (type) {
        case BonusType::SHIELD:
            player.activateShield();
            std::cout << "Shield activated!" << std::endl;
            break;
            
        case BonusType::MEGA_SHOT:
            player.activateMegaShot();
            std::cout << "Mega shot activated!" << std::endl;
            break;
            
        case BonusType::EXTRA_LIFE:
            player.addLife();
            std::cout << "Extra life gained!" << std::endl;
            break;
            
        case BonusType::FREEZE_ENEMIES:
            for (auto& enemy : enemies) {
                // Freeze enemies for 2 seconds
                // This would need enemy freeze state implementation
            }
            std::cout << "Enemies frozen!" << std::endl;
            break;
            
        case BonusType::SPEED_BOOST:
            // Speed boost implementation
            std::cout << "Speed boost activated!" << std::endl;
            break;
    }
}

void Level::checkCompletion() {
    if (enemiesKilled >= totalEnemies && enemies.empty() && currentWave >= totalWaves) {
        completed = true;
        std::cout << "Level completed!" << std::endl;
    }
}

int Level::getRemainingEnemies() const {
    return totalEnemies - enemiesKilled;
}
