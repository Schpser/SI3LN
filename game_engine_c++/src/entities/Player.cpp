#include "../include/Player.hpp"
#include <iostream>
#include <ctime>

Player::Player(float x, float y, int characterId)
    : Entity(x, y, Constants::PLAYER_WIDTH, Constants::PLAYER_HEIGHT, EntityType::PLAYER),
      lives(Constants::MAX_LIVES), score(0), characterId(characterId),
      speed(Constants::PLAYER_SPEED), lastShotTime(0), 
      shootCooldown(Constants::PLAYER_SHOOT_COOLDOWN),
      hasShield(false), shieldStartTime(0),
      hasMegaShot(false), megaShotStartTime(0),
      isFrozen(false), isBlinded(false), isRooted(false),
      debuffStartTime(0), debuffDuration(0) {
    textureId = "player_" + std::to_string(characterId);
}

void Player::update(float deltaTime) {
    if (!active) return;
    
    unsigned int currentTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    
    // Update debuffs
    updateDebuffs(currentTime);
    
    // Update bonuses
    updateBonuses(currentTime);
    
    // Apply velocity if not frozen
    if (!isFrozen) {
        position += velocity * deltaTime;
    }
}

void Player::render() {
    if (!active) return;
    
    // Rendering will be handled by the graphics system
    // This is just a placeholder for the game logic
    std::cout << "Rendering player at (" << position.x << ", " << position.y << ")" << std::endl;
}

bool Player::canShoot() const {
    if (!active || isFrozen) return false;
    
    unsigned int currentTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    return (currentTime - lastShotTime) >= shootCooldown;
}

void Player::shoot() {
    if (canShoot()) {
        lastShotTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
        // Bullet creation will be handled by the game manager
    }
}

void Player::moveLeft() {
    if (!isRooted && !isFrozen) {
        velocity.x = -speed;
    }
}

void Player::moveRight() {
    if (!isRooted && !isFrozen) {
        velocity.x = speed;
    }
}

void Player::moveUp() {
    if (!isRooted && !isFrozen) {
        velocity.y = -speed;
    }
}

void Player::moveDown() {
    if (!isRooted && !isFrozen) {
        velocity.y = speed;
    }
}

void Player::stopMovement() {
    velocity.x = 0;
    velocity.y = 0;
}

void Player::addLife() {
    lives++;
}

void Player::loseLife() {
    if (!hasShield) {
        lives--;
        if (lives <= 0) {
            active = false;
        }
    } else {
        // Shield absorbs the hit
        hasShield = false;
    }
}

void Player::addScore(int points) {
    score += points;
}

void Player::activateShield() {
    hasShield = true;
    shieldStartTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
}

void Player::activateMegaShot() {
    hasMegaShot = true;
    megaShotStartTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
}

bool Player::hasShieldActive() const {
    return hasShield;
}

bool Player::hasMegaShotActive() const {
    return hasMegaShot;
}

void Player::updateBonuses(unsigned int currentTime) {
    // Check shield duration
    if (hasShield && (currentTime - shieldStartTime) >= Constants::SHIELD_DURATION) {
        hasShield = false;
    }
    
    // Check mega shot duration
    if (hasMegaShot && (currentTime - megaShotStartTime) >= Constants::MEGA_SHOT_DURATION) {
        hasMegaShot = false;
    }
}

void Player::applyFreeze(unsigned int duration) {
    isFrozen = true;
    debuffStartTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    debuffDuration = duration;
    velocity = Vector2D(0, 0);
}

void Player::applyBlind(unsigned int duration) {
    isBlinded = true;
    debuffStartTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    debuffDuration = duration;
}

void Player::applyRoot(unsigned int duration) {
    isRooted = true;
    debuffStartTime = static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC);
    debuffDuration = duration;
    velocity = Vector2D(0, 0);
}

void Player::clearDebuffs() {
    isFrozen = false;
    isBlinded = false;
    isRooted = false;
    debuffDuration = 0;
}

void Player::updateDebuffs(unsigned int currentTime) {
    if ((isFrozen || isBlinded || isRooted) && 
        (currentTime - debuffStartTime) >= debuffDuration) {
        clearDebuffs();
    }
}
