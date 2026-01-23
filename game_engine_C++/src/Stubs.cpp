// Stubs.cpp
// Fichier contenant des classes stubs (bouchons) pour des implémentations futures du moteur de jeu
#include <SDL2/SDL.h> // Inclusion de la bibliothèque SDL2 pour le rendu et les entrées
#include <string>	  // Inclusion pour la gestion des chaînes de caractères

namespace SI3LN
{

	// Classe stub pour la gestion du rendu graphique
	class Renderer
	{
	public:
		Renderer([[maybe_unused]] SDL_Renderer *renderer) {} // Constructeur prenant un pointeur vers le renderer SDL (non utilisé ici)
	};

	// Classe stub pour la gestion des entrées utilisateur (clavier, souris, etc.)
	class InputManager
	{
	public:
		void update() {} // Méthode pour mettre à jour les entrées (vide pour le stub)
	};

	// Classe stub pour la gestion des collisions entre entités
	class CollisionManager
	{
	public:
		void checkCollisions() {} // Méthode pour vérifier les collisions (vide pour le stub)
	};

	// Classe stub pour la gestion des entités du jeu (joueurs, ennemis, etc.)
	class EntityManager
	{
	public:
		void update() {} // Méthode pour mettre à jour les entités (vide pour le stub)
	};

	// Classe stub pour la gestion des ressources (textures, sons, etc.)
	class AssetManager
	{
	public:
		AssetManager([[maybe_unused]] SDL_Renderer *renderer) {} // Constructeur prenant un pointeur vers le renderer SDL (non utilisé ici)
	};

	// Classe stub pour la gestion de l'audio (musique, effets sonores)
	class AudioManager
	{
	public:
		void playSound([[maybe_unused]] const std::string &name) {} // Méthode pour jouer un son en fonction de son nom (vide pour le stub)
	};

	// Classe stub pour la gestion du score du joueur
	class ScoreManager
	{
	public:
		void addScore([[maybe_unused]] int score) {} // Méthode pour ajouter des points au score (vide pour le stub)
	};

	// Classe stub pour la gestion de l'authentification utilisateur
	class AuthSystem
	{
	public:
		bool login([[maybe_unused]] const std::string &user, [[maybe_unused]] const std::string &pass) { return true; } // Méthode de connexion simulée (retourne toujours vrai)
	};

} // namespace SI3LN // Fin de l'espace de noms SI3LN
