#pragma once

#include "Entity.h"
#include "Constants.h"

namespace SI3LN {

class Bullet : public Entity {
public:
    Bullet(float x, float y, bool isPlayerBullet, int screenHeight, const Color& color);
    ~Bullet() override = default;
    
    void update(float deltaTime) override;
    void render(SDL_Renderer* renderer) override;
    
    bool isPlayerBullet() const { return playerBullet; }
    
private:
    bool playerBullet;
    int screenHeight;
    float speed;
    Color color;
};

} // namespace SI3LN