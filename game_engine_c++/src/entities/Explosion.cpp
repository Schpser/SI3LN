#include "../include/Explosion.hpp"
#include <iostream>
#include <ctime>

Explosion::Explosion(float x, float y, unsigned int duration)
    : position(x, y), width(60), height(60),
      startTime(static_cast<unsigned int>(clock() * 1000 / CLOCKS_PER_SEC)),
      duration(duration), active(true), textureId("explosion"), scale(1.0f) {
}

void Explosion::update(unsigned int currentTime) {
    if (!active) return;
    
    unsigned int elapsed = currentTime - startTime;
    
    if (elapsed >= duration) {
        active = false;
    } else {
        // Scale up the explosion over time
        float progress = static_cast<float>(elapsed) / duration;
        scale = 1.0f + progress * 0.5f;
    }
}

void Explosion::render() {
    if (!active) return;
    
    std::cout << "Rendering explosion at (" << position.x << ", " << position.y 
              << ") scale: " << scale << std::endl;
}
