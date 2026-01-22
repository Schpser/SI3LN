#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include <memory>

class Player : public Entity {
private:
    int lives;
    int score;
    int characterId;
    float speed;
    unsigned int lastShotTime;
    unsigned int shootCooldown;
    
    // Bonus states
    bool hasShield;
    unsigned int shieldStartTime;
    bool hasMegaShot;
    unsigned int megaShotStartTime;
    
    // Debuff states
    bool isFrozen;
    bool isBlinded;
    bool isRooted;
    unsigned int debuffStartTime;
    unsigned int debuffDuration;
    
public:
    Player(float x, float y, int characterId);
    ~Player() override = default;
    
    void update(float deltaTime) override;
    void render() override;
    
    // Player actions
    bool canShoot() const;
    void shoot();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void stopMovement();
    
    // Lives and score
    void addLife();
    void loseLife();
    int getLives() const { return lives; }
    void addScore(int points);
    int getScore() const { return score; }
    void resetScore() { score = 0; }
    
    // Bonuses
    void activateShield();
    void activateMegaShot();
    bool hasShieldActive() const;
    bool hasMegaShotActive() const;
    void updateBonuses(unsigned int currentTime);
    
    // Debuffs
    void applyFreeze(unsigned int duration);
    void applyBlind(unsigned int duration);
    void applyRoot(unsigned int duration);
    void clearDebuffs();
    bool isFrozenState() const { return isFrozen; }
    bool isBlindedState() const { return isBlinded; }
    bool isRootedState() const { return isRooted; }
    void updateDebuffs(unsigned int currentTime);
    
    // Getters
    int getCharacterId() const { return characterId; }
    void setCharacterId(int id) { characterId = id; }
};

#endif // PLAYER_HPP
