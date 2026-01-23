// Vector2D.h

#pragma once

#include <cmath>

namespace SI3LN {

// Structure représentant un vecteur 2D
struct Vector2D {
    float x, y;
    
    // Constructeur initialisant les composantes du vecteur
    Vector2D(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
    
    // Opérateurs
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y); // Additionne deux vecteurs
    }
    
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y); // Soustrait deux vecteurs
    }
    
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar); // Multiplie un vecteur par un scalaire
    }
    
    Vector2D operator/(float scalar) const {
        return Vector2D(x / scalar, y / scalar); // Divise un vecteur par un scalaire
    }
    
    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this; // Ajoute un vecteur à l'autre et retourne le résultat
    }
    
    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this; // Soustrait un vecteur de l'autre et retourne le résultat
    }
    
    // Méthodes utilitaires
    float length() const {
        return std::sqrt(x * x + y * y); // Calcule la longueur du vecteur (norme)
    }
    
    float lengthSquared() const {
        return x * x + y * y; // Calcule le carré de la longueur du vecteur
    }
    
    Vector2D normalized() const {
        float len = length();
        if (len > 0.0f) {
            return Vector2D(x / len, y / len); // Normalise le vecteur et retourne le résultat
        }
        return Vector2D(0.0f, 0.0f); // Retourne un vecteur nul si la longueur est nulle
    }
    
    void normalize() {
        float len = length();
        if (len > 0.0f) {
            x /= len;
            y /= len; // Normalise le vecteur en place
        }
    }
    
    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y; // Calcule le produit scalaire avec un autre vecteur
    }
    
    float distance(const Vector2D& other) const {
        return (*this - other).length(); // Calcule la distance entre deux points (longueur du vecteur différence)
    }
    
    float distanceSquared(const Vector2D& other) const {
        return (*this - other).lengthSquared(); // Calcule le carré de la distance entre deux points
    }
};

} // namespace SI3LN