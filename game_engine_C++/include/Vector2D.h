#pragma once

#include <cmath>

namespace SI3LN {

struct Vector2D {
    float x, y;
    
    Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    
    // Operators
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }
    
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }
    
    Vector2D operator/(float scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }
    
    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    // Utility methods
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    
    float lengthSquared() const {
        return x * x + y * y;
    }
    
    Vector2D normalized() const {
        float len = length();
        if (len > 0.0f) {
            return Vector2D(x / len, y / len);
        }
        return Vector2D(0.0f, 0.0f);
    }
    
    void normalize() {
        float len = length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
        }
    }
    
    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
    
    float distance(const Vector2D& other) const {
        return (*this - other).length();
    }
    
    float distanceSquared(const Vector2D& other) const {
        return (*this - other).lengthSquared();
    }
};

} // namespace SI3LN