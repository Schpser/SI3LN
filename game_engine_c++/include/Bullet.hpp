#ifndef BULLET_HPP
#define BULLET_HPP

#include "Entity.hpp"

class Bullet : public Entity {
private:
    int damage;
    bool isPlayerBullet;
    float speed;
    
public:
    Bullet(float x, float y, bool isPlayerBullet, int damage = 1);
    ~Bullet() override = default;
    
    void update(float deltaTime) override;
    void render() override;
    
    // Getters
    int getDamage() const { return damage; }
    bool isFromPlayer() const { return isPlayerBullet; }
    
    // Check if bullet should be removed
    bool shouldRemove(int screenHeight) const;
};

#endif // BULLET_HPP
