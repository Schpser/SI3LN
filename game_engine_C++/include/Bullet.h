#pragma once // Permet d'éviter les inclusions multiples du fichier

#include "Entity.h" // Inclusion de la classe de base Entity
#include "Constants.h" // Inclusion des constantes et types utilisés

namespace SI3LN {

// Classe représentant un projectile (bullet) dans le jeu
class Bullet : public Entity {
public:
    // Constructeur : initialise la position, le type, la hauteur d'écran et la couleur du projectile
    Bullet(float x, float y, bool isPlayerBullet, int screenHeight, const Color& color);
    ~Bullet() override = default; // Destructeur par défaut
    
    // Met à jour la position et l'état du projectile à chaque frame
    void update(float deltaTime) override;
    // Affiche le projectile à l'écran
    void render(SDL_Renderer* renderer) override;
    
    // Indique si le projectile appartient au joueur
    bool isPlayerBullet() const { return playerBullet; }
    
private:
    bool playerBullet; // Vrai si le projectile appartient au joueur, faux si c'est un ennemi
    int screenHeight; // Hauteur de l'écran pour gérer la sortie du projectile
    float speed; // Vitesse de déplacement du projectile
    Color color; // Couleur du projectile
};

} // namespace SI3LN // Fin de l'espace de noms SI3LN