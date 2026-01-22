#ifndef BONUS_HPP
#define BONUS_HPP

#include "Entity.hpp"
#include "Constants.hpp"

class Bonus : public Entity {
private:
    BonusType bonusType;
    float fallSpeed;
    unsigned int spawnTime;
    unsigned int lifetime; // How long the bonus stays on screen
    
public:
    Bonus(float x, float y, BonusType type);
    ~Bonus() override = default;
    
    void update(float deltaTime) override;
    void render() override;
    
    // Getters
    BonusType getBonusType() const { return bonusType; }
    
    // Check if bonus should be removed
    bool hasExpired(unsigned int currentTime) const;
    bool isOffScreen(int screenHeight) const;
    
    // Get bonus description
    std::string getDescription() const;
};

#endif // BONUS_HPP
