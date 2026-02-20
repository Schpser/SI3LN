#pragma once // Évite les inclusions multiples du fichier

#include "Vector2D.h" // Inclusion de la classe pour la gestion des vecteurs 2D
#include <SDL2/SDL.h> // Inclusion pour la gestion des rectangles et du rendu
#include <memory> // Pour l'utilisation de std::shared_ptr

namespace SI3LN {

// Classe de base représentant une entité du jeu (joueur, ennemi, projectile, etc.)
class Entity {
public:
    // Constructeur : initialise la position, la taille et l'état de vie de l'entité
    Entity(float x, float y, int width, int height)
        : position(x, y), velocity(0.0f, 0.0f)
        , width(width), height(height), alive(true) {}
    
    virtual ~Entity() = default; // Destructeur virtuel par défaut
    
    // Méthodes virtuelles pures à implémenter dans les classes dérivées
    virtual void update(float deltaTime) = 0; // Mise à jour de l'entité à chaque frame
    virtual void render(SDL_Renderer* renderer) = 0; // Affichage de l'entité
    
    // ==================== Getters ====================
    Vector2D getPosition() const { return position; } // Retourne la position de l'entité
    Vector2D getVelocity() const { return velocity; } // Retourne la vitesse de l'entité
    SDL_Rect getRect() const { // Retourne le rectangle englobant l'entité
        return {
            static_cast<int>(position.x - width / 2), // Calcul de la position X du coin supérieur gauche
            static_cast<int>(position.y - height / 2), // Calcul de la position Y du coin supérieur gauche
            width, // Largeur du rectangle
            height // Hauteur du rectangle
        };
    }
    int getWidth() const { return width; } // Retourne la largeur
    int getHeight() const { return height; } // Retourne la hauteur
    bool isAlive() const { return alive; } // Indique si l'entité est vivante
    
    // ==================== Setters ====================
    void setPosition(const Vector2D& pos) { position = pos; } // Modifie la position
    void setVelocity(const Vector2D& vel) { velocity = vel; } // Modifie la vitesse
    void kill() { alive = false; } // Tue l'entité (la rend inactive)
    
    // ==================== Détection de collision ====================
    // Vérifie si cette entité intersecte une autre entité
    bool intersects(const Entity& other) const {
        SDL_Rect thisRect = getRect();
        SDL_Rect otherRect = other.getRect();
        return SDL_HasIntersection(&thisRect, &otherRect);
    }
    // Calcule la distance à une autre entité
    float distanceTo(const Entity& other) const {
        return position.distance(other.position);
    }

protected:
    Vector2D position; // Position de l'entité dans l'espace de jeu
    Vector2D velocity; // Vitesse de l'entité
    int width; // Largeur de l'entité
    int height; // Hauteur de l'entité
    bool alive; // Indique si l'entité est active/vivante
};

using EntityPtr = std::shared_ptr<Entity>; // Alias pour un pointeur partagé vers une entité

} // namespace SI3LN // Fin de l'espace de noms SI3LN