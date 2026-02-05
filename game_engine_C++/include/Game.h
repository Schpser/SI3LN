#pragma once // Évite les inclusions multiples du fichier

#include "Constants.h"		// Inclusion des constantes et types globaux
#include "Player.h"			// Inclusion de la classe Player
#include "Enemy.h"			// Inclusion de la classe Enemy
#include "Bullet.h"			// Inclusion de la classe Bullet
#include <SDL2/SDL.h>		// Inclusion pour la gestion de la fenêtre et du rendu
#include <SDL2/SDL_image.h> // Inclusion pour la gestion des images
#include <SDL2/SDL_ttf.h>	// Inclusion pour la gestion des polices
#include <vector>			// Pour l'utilisation des tableaux dynamiques
#include <memory>			// Pour l'utilisation de std::unique_ptr et std::shared_ptr
#include <string>			// Pour la gestion des chaînes de caractères

namespace SI3LN
{

	// Classe principale qui gère le cycle de vie et la logique du jeu
	class Game
	{
	public:
		Game();	 // Constructeur
		~Game(); // Destructeur

		// Boucle principale du jeu
		void run();
		
		// Définir le monde et le niveau (appelé depuis Python)
		void setWorldAndLevel(const std::string& world, int level);

		// Définir le personnage sélectionné (appelé depuis Python)
		void setPlayerIndex(int index) { playerIndex = index; }

		// Mode d'affichage de la fenêtre (fenêtré, fenêtré sans bord, ou plein écran)
		enum class WindowMode { WINDOWED = 0, BORDERLESS, FULLSCREEN };

		// Définir le mode d'affichage de la fenêtre (publiquement accessible)
		void setWindowMode(WindowMode mode);

	private:
		// ==================== Initialisation et nettoyage ====================
		bool init();	   // Initialise SDL et les ressources
		void loadAssets(); // Charge les textures et polices
		void cleanup();	   // Libère toutes les ressources

		// ==================== Boucle de jeu ====================
		void handleEvents();		  // Gère les événements (clavier, souris, fenêtre)
		void update(float deltaTime); // Met à jour la logique du jeu
		void render();				  // Affiche le jeu à l'écran

		// ==================== Gameplay ====================
		void updateGameplay(float deltaTime); // Met à jour la logique du gameplay
		void updateMainMenu(float deltaTime); // Met à jour la logique du menu principal
		void updatePause(float deltaTime);	  // Met à jour la logique de pause
		void updateGameOver(float deltaTime); // Met à jour la logique de game over
		void renderGameplay();				  // Affiche les éléments du gameplay
		void spawnEnemies();				  // Crée les ennemis pour le niveau
		void checkCollisions();				  // Vérifie les collisions entre entités

		// ==================== Gestion des entrées ====================
		void handleKeyPress(SDL_Keycode key); // Gère les touches du clavier
		void handleMouseClick(int x, int y);  // Gère les clics de souris

		// ==================== Gestion des états ====================
		void changeState(GameState newState); // Change l'état du jeu

		// ==================== Composants SDL ====================
		SDL_Window *window;		// Pointeur vers la fenêtre SDL
		SDL_Renderer *renderer; // Pointeur vers le moteur de rendu SDL

		// ==================== État du jeu ====================
		GameState currentState; // État actuel du jeu
		bool running;            // Indique si le jeu est en cours d'exécution
		WindowMode windowMode;   // Mode courant de la fenêtre (stocké en privé)


		// ==================== Dimensions de l'écran ====================
		int screenWidth;  // Largeur de la fenêtre
		int screenHeight; // Hauteur de la fenêtre

		// ==================== Gestion du temps ====================
		uint32_t lastFrameTime; // Temps de la dernière frame

		// ==================== Données du jeu ====================
		int currentScore;		  // Score actuel du joueur
		int currentLevel;		  // Niveau actuel
		std::string currentWorld; // Monde actuel
		std::string currentWorldLower; // Monde en minuscules (pour le background)
		int lives;				  // Nombre de vies restantes

		// ==================== Entités du jeu ====================
		std::unique_ptr<Player> player;						// Joueur
		std::vector<std::shared_ptr<Enemy>> enemies;		// Liste des ennemis
		std::vector<std::shared_ptr<Bullet>> playerBullets; // Projectiles du joueur
		std::vector<std::shared_ptr<Bullet>> enemyBullets;	// Projectiles des ennemis

		// ==================== Ressources graphiques et polices ====================
		SDL_Texture *playerTexture;		// Texture du joueur
		SDL_Texture *enemyTexture;		// Texture des ennemis (single, legacy)
		std::vector<SDL_Texture*> enemyTextures; // Toutes les textures d'ennemis du monde courant
		SDL_Texture *backgroundTexture; // Texture de l'arrière-plan
		TTF_Font *font;					// Police de taille normale
		TTF_Font *fontLarge;			// Police de grande taille

		// ==================== Fonctions utilitaires ====================
		SDL_Texture *loadTexture(const std::string &path);											// Charge une texture à partir d'un chemin
		void renderText(const std::string &text, int x, int y, TTF_Font *font, const Color &color); // Affiche du texte à l'écran
		// ==================== Intro de niveau ====================
		bool showLevelIntro = false;           // Indique si le texte d'intro de niveau est affiché
		float levelIntroTimer = 0.0f;         // Compteur pour la durée d'affichage
		static constexpr float LEVEL_INTRO_DURATION = 3.0f; // Durée par défaut en secondes
		// ==================== Index du personnage sélectionné ====================
		int playerIndex = 0; // Index du personnage sélectionné
	};

} // namespace SI3LN // Fin de l'espace de noms SI3LN
