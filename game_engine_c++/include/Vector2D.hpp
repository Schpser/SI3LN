#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

#include <cmath>

struct Vector2D {
    float x, y;
    
    Vector2D() : x(0.0f), y(0.0f) {}
    Vector2D(float x, float y) : x(x), y(y) {}
    
    // Vector operations
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
    
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    
    float lengthSquared() const {
        return x * x + y * y;
    }
    
    Vector2D normalized() const {
        float len = length();
        if (len > 0) {
            return Vector2D(x / len, y / len);
        }
        return Vector2D(0, 0);
    }
    
    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
    
    float distance(const Vector2D& other) const {
        return (*this - other).length();
    }
};

#endif // VECTOR2D_HPP
