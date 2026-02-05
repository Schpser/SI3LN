
#include "Game.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <vector>

namespace SI3LN {

void Game::spawnEnemies()
{
	// Nettoyer les ennemis existants
	enemies.clear();

	// Paramètres de la vague
	const int nbCols = 8;
	const int nbRows = 3;
	const float spacingX = 80.0f;
	const float spacingY = 60.0f;
	const float startX = 100.0f;
	const float startY = 60.0f;

	// Utiliser la première texture d'ennemi disponible
	SDL_Texture* tex = nullptr;
	if (!enemyTextures.empty())
		tex = enemyTextures[0];

	for (int row = 0; row < nbRows; ++row) {
		for (int col = 0; col < nbCols; ++col) {
			float x = startX + col * spacingX;
			float y = startY + row * spacingY;
			enemies.push_back(std::make_shared<Enemy>(x, y, tex, screenWidth, screenHeight));
		}
	}
	std::cout << "[DEBUG] Spawned " << enemies.size() << " enemies" << std::endl;
}

// GESTION DES ÉVÉNEMENTS CLAVIER
void Game::handleKeyPress(SDL_Keycode key)
{
	std::cout << "[DEBUG] Touche pressée: " << SDL_GetKeyName(key) << " (" << key << ")" << std::endl;
	switch (key)
	{
	case SDLK_ESCAPE:
		if (currentState == GameState::GAMEPLAY)
		{
			changeState(GameState::PAUSE);
		}
		else if (currentState == GameState::PAUSE)
		{
			changeState(GameState::GAMEPLAY);
		}
		else
		{
			running = false;
		}
		break;
       case SDLK_RETURN:
	       if (currentState == GameState::MAIN_MENU)
	       {
		       changeState(GameState::GAMEPLAY);
		       spawnEnemies();
		       player = std::make_unique<Player>(
			       screenWidth / 2.0f,
			       screenHeight - 100.0f,
			       playerTexture,
			       screenWidth,
			       screenHeight
		       );
	       }
	       break;
	case SDLK_q:
		if (currentState == GameState::PAUSE)
		{
			running = false;
		}
		break;

	case SDLK_F11:
		// Cycle entre WINDOWED -> BORDERLESS -> FULLSCREEN
		if (windowMode == WindowMode::WINDOWED)
		{
			setWindowMode(WindowMode::BORDERLESS);
		}
		else if (windowMode == WindowMode::BORDERLESS)
		{
			setWindowMode(WindowMode::FULLSCREEN);
		}
		else
		{
			setWindowMode(WindowMode::WINDOWED);
		}
		break;

	case SDLK_SPACE:
		// Le tir continu est géré dans updateGameplay
		break;

	       case SDLK_LSHIFT:
		       if (currentState == GameState::GAMEPLAY && player && player->useSpecial())
		       {
			       auto bullet = std::make_shared<Bullet>(
				       player->getPosition().x,
				       player->getPosition().y - player->getHeight() / 2,
				       true,
				       screenHeight,
				       Colors::YELLOW
			       );
					   playerBullets.push_back(bullet);
		       }
		       if (currentState == GameState::MAIN_MENU)
		       {
			       changeState(GameState::GAMEPLAY);
			       spawnEnemies();
			       player = std::make_unique<Player>(
				       screenWidth / 2.0f,
				       screenHeight - 100.0f,
				       playerTexture,
				       screenWidth,
				       screenHeight
			       );
		       }
		       break;


	default:
		 break;
	}
}

// GESTION DES CLICS SOURIS
void Game::handleMouseClick(int x, int y)
{
	// À compléter : logique de gestion des clics souris
}

// MISE À JOUR GÉNÉRALE DU JEU
void Game::update(float deltaTime)
{
	if (currentState != GameState::GAMEPLAY)
		return;

	// Récupérer l'état du clavier pour le mouvement continu
	const uint8_t* keyState = SDL_GetKeyboardState(nullptr);

	// Mettre à jour le joueur (mouvement avec les touches directionnelles)
	if (player) {
		player->handleInput(keyState);
		player->update(deltaTime);

		// Tir automatique avec ESPACE
		if (keyState[SDL_SCANCODE_SPACE] && player->canShoot()) {
			auto bullet = std::make_shared<Bullet>(
				player->getPosition().x,
				player->getPosition().y - player->getHeight() / 2,
				true,
				screenHeight,
				Colors::CYAN
			);
			playerBullets.push_back(bullet);
			player->resetShootCooldown();
		}
	}

	// Mettre à jour les ennemis
	for (auto& enemy : enemies) {
		enemy->update(deltaTime);
	}

	// Mettre à jour les projectiles du joueur
	for (auto& bullet : playerBullets) {
		bullet->update(deltaTime);
	}

	// Supprimer les projectiles hors écran
	playerBullets.erase(
		std::remove_if(playerBullets.begin(), playerBullets.end(),
			[](const std::shared_ptr<Bullet>& b) { return !b->isAlive(); }),
		playerBullets.end()
	);

	// Gérer l'affichage d'intro de niveau
	if (showLevelIntro)
	{
		levelIntroTimer -= deltaTime;
		if (levelIntroTimer <= 0.0f)
		{
			showLevelIntro = false;
			levelIntroTimer = 0.0f;
		}
	}
}

	// Constructeur par défaut de la classe Game qui initialise les membres
	Game::Game()
		: window(nullptr) // Pointeur de la fenêtre SDL
		,
		renderer(nullptr) // Pointeur du moteur de rendu SDL
		,
		currentState(GameState::MAIN_MENU) // État initial du jeu (menu principal)
		,
		running(false) // Booléen indiquant si le jeu est en cours d'exécution
		,
		windowMode(WindowMode::WINDOWED) // Mode de la fenêtre (fenêtré par défaut)
		,
		screenWidth(DEFAULT_SCREEN_WIDTH) // Largeur de l'écran
		,
		screenHeight(DEFAULT_SCREEN_HEIGHT) // Hauteur de l'écran
		,
		lastFrameTime(0) // Temps de la dernière image (frame)
		,
		currentScore(0) // Score actuel du joueur
		,
		currentLevel(1) // Niveau actuel du jeu
		,
		currentWorld("Space") // Monde actuel du jeu
		,
		lives(MAX_LIVES) // Nombre de vies disponibles
		,
		playerTexture(nullptr) // Texture du joueur
		,
		enemyTexture(nullptr) // Texture des ennemis
		,
		backgroundTexture(nullptr) // Texture de l'arrière-plan
		,
		font(nullptr) // Police de caractères de taille normale
		,
		fontLarge(nullptr) // Police de caractères de grande taille
		,
		showLevelIntro(false)
		,
		levelIntroTimer(0.0f)
	{
	}

	// Définir le monde et le niveau (appelé depuis Python)
	void Game::setWorldAndLevel(const std::string& world, int level)
	{
		currentWorld = world;
		currentWorld = world; // Garde la casse d'origine pour les ennemis
		// Stocker aussi la version minuscule pour le background
		currentWorldLower = world;
		std::transform(currentWorldLower.begin(), currentWorldLower.end(), currentWorldLower.begin(), [](unsigned char c){ return std::tolower(c); });
		currentLevel = level;
		// Lancer l'affichage de l'intro de niveau
		showLevelIntro = true;
		levelIntroTimer = LEVEL_INTRO_DURATION;
		std::cout << "Game configured: World=" << currentWorld << ", Level=" << currentLevel << std::endl;
	}

	// Destructeur qui appelle cleanup pour libérer les ressources
	Game::~Game()
	{
		cleanup();
	}

	bool Game::init()
	{
		// Initialiser SDL avec le support vidéo et audio
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{
			std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
			return false;
		}

		// Initialiser SDL_image pour charger les images PNG et JPG
		int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
			return false;
		}

		// Initialiser SDL_ttf pour le rendu du texte avec les polices TTF
		if (TTF_Init() == -1)
		{
			std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
			return false;
		}

		// Créer la fenêtre principale du jeu avec les paramètres spécifiés
		window = SDL_CreateWindow(
			"S I 3 L N - Space Invaders III Last Night",
			SDL_WINDOWPOS_CENTERED,					// Centrer la fenêtre horizontalement
			SDL_WINDOWPOS_CENTERED,					// Centrer la fenêtre verticalement
			screenWidth,							// Largeur de la fenêtre
			screenHeight,							// Hauteur de la fenêtre
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE // Afficher la fenêtre et la rendre redimensionnable
		);

		// Vérifier si la création de la fenêtre a échoué
		if (!window)
		{
			std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
			return false;
		}

		// Créer le moteur de rendu avec accélération matérielle et synchronisation verticale
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer)
		{
			std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
			return false;
		}

		// Activer le blending (fusion) pour la transparence des éléments
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		// Charger les ressources (textures, polices)
		loadAssets();

		std::cout << "Game initialized successfully!" << std::endl;
		// Appliquer le mode de fenêtre demandé (par défaut WINDOWED)
		setWindowMode(windowMode);

		return true;
	}



	// Définir le mode de la fenêtre (fenêtré, sans bord, plein écran)
	void Game::setWindowMode(Game::WindowMode mode)
	{
		if (!window) { windowMode = mode; return; }
		windowMode = mode;
		SDL_DisplayMode dm;
		switch (mode)
		{
		case Game::WindowMode::WINDOWED:
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowBordered(window, SDL_TRUE);
			SDL_SetWindowSize(window, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
			screenWidth = DEFAULT_SCREEN_WIDTH;
			screenHeight = DEFAULT_SCREEN_HEIGHT;
			std::cout << "Window mode: WINDOWED" << std::endl;
			break;

		case Game::WindowMode::BORDERLESS:
			// Mode fenêtré sans bord -> utilise SDL_WINDOW_FULLSCREEN_DESKTOP
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			if (SDL_GetDesktopDisplayMode(0, &dm) == 0)
			{
				screenWidth = dm.w;
				screenHeight = dm.h;
			}
			std::cout << "Window mode: BORDERLESS (desktop)" << std::endl;
			break;

		case Game::WindowMode::FULLSCREEN:
			// Essaye le fullscreen exclusif, sinon fallback en fullscreen desktop
			if (SDL_GetDesktopDisplayMode(0, &dm) == 0)
			{
				SDL_SetWindowDisplayMode(window, &dm);
			}
			if (SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN) != 0)
			{
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			SDL_GetWindowSize(window, &screenWidth, &screenHeight);
			std::cout << "Window mode: FULLSCREEN" << std::endl;
			break;

		default:
			break;
		}
	}

	void Game::loadAssets()
	{
		std::cout << "Loading assets..." << std::endl;

		// Charger les polices de caractères pour le texte du jeu
		font = TTF_OpenFont("assets/fonts/edunline/SuperPixel-m2L8j.ttf", FONT_SIZE_MEDIUM);    // Police de taille moyenne
		fontLarge = TTF_OpenFont("assets/fonts/edunline/SuperPixel-m2L8j.ttf", FONT_SIZE_LARGE); // Police de grande taille

		if (!font || !fontLarge)
		{
			std::cerr << "Warning: Could not load fonts. Using system default." << std::endl;
		}

		// Mapping des mondes pour les dossiers
		std::string worldKey = currentWorld;
		if (worldKey == "Apocalyptic") worldKey = "Apocalyptic_world";
		else if (worldKey == "Desert") worldKey = "Desert_world";
		else if (worldKey == "Forest") worldKey = "Forest_world";
		else if (worldKey == "Marine") worldKey = "Marine_world";
		else if (worldKey == "Space") worldKey = "Space_world";

		// Charger la texture du joueur
		std::string playerPath = "assets/players/player_" + std::to_string(playerIndex + 1) + "/AnimateDiff_00001.001.png";
		playerTexture = loadTexture(playerPath);

		// Charger toutes les textures d'ennemis disponibles (enemy*.png)
		this->enemyTextures.clear();
		std::string enemyDir = "assets/enemies/" + worldKey + "/";
		for (const auto& entry : std::filesystem::directory_iterator(enemyDir)) {
			std::string name = entry.path().filename().string();
			if (name.find("enemy") == 0 && entry.path().extension() == ".png") {
				SDL_Texture* tex = loadTexture(entry.path().string());
				if (tex) this->enemyTextures.push_back(tex);
			}
		}
		if (this->enemyTextures.empty()) {
			std::cerr << "Aucune texture d'ennemi trouvée dans " << enemyDir << std::endl;
		}

		// Charger le background du monde (détection dynamique de l'extension, insensible à la casse)
		std::string bgBase = "assets/worlds/background_" + currentWorldLower;
		backgroundTexture = loadTexture(bgBase + ".jpg");
		if (!backgroundTexture) {
			backgroundTexture = loadTexture(bgBase + ".png");
		}
		if (!backgroundTexture && !currentWorldLower.empty()) {
			std::string worldCap = currentWorldLower;
			worldCap[0] = std::toupper(worldCap[0]);
			std::string bgBaseCap = "assets/worlds/background_" + worldCap;
			backgroundTexture = loadTexture(bgBaseCap + ".jpg");
			if (!backgroundTexture) {
				backgroundTexture = loadTexture(bgBaseCap + ".png");
			}
		}

		std::cout << "Assets loaded!" << std::endl;
	}

	// Fonction qui charge une texture à partir d'un chemin d'accès
	SDL_Texture *Game::loadTexture(const std::string &path)
	{
		// Charger la texture avec SDL_image
		SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
		// Vérifier si le chargement a échoué et afficher un message d'erreur
		if (!texture)
		{
			std::cerr << "Warning: Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
		}
		// Retourner la texture (peut être nullptr si elle n'a pas pu être chargée)
		return texture;
	}

	// Fonction de nettoyage qui libère toutes les ressources allouées
	void Game::cleanup()
	{
		// Détruire les textures chargées
		if (playerTexture)
			SDL_DestroyTexture(playerTexture); // Détruire la texture du joueur
		if (enemyTexture)
			SDL_DestroyTexture(enemyTexture); // Détruire la texture des ennemis
		if (backgroundTexture)
			SDL_DestroyTexture(backgroundTexture); // Détruire la texture de l'arrière-plan

		// Fermer les polices de caractères
		if (font)
			TTF_CloseFont(font); // Fermer la police normale
		if (fontLarge)
			TTF_CloseFont(fontLarge); // Fermer la grande police

		// Détruire le moteur de rendu et la fenêtre SDL
		if (renderer)
			SDL_DestroyRenderer(renderer); // Libérer le moteur de rendu
		if (window)
			SDL_DestroyWindow(window); // Libérer la fenêtre

		// Arrêter les sous-systèmes SDL
		TTF_Quit(); // Arrêter SDL_ttf
		IMG_Quit(); // Arrêter SDL_image
		SDL_Quit(); // Arrêter SDL

		std::cout << "Game cleaned up successfully!" << std::endl;
	}

	// Fonction principale qui lance la boucle de jeu
	void Game::run()
	{
		// Initialiser le jeu et vérifier que tout s'est bien passé
		if (!init())
		{
			std::cerr << "Failed to initialize game!" << std::endl;
			return;
		}

		// Marquer le jeu comme en cours d'exécution
		running = true;
		// Enregistrer le temps du premier frame
		lastFrameTime = SDL_GetTicks();

		// Boucle principale du jeu qui s'exécute tant que running est vrai
		while (running)
		{
			// Obtenir le temps actuel en millisecondes
			uint32_t currentTime = SDL_GetTicks();
			// Calculer le temps écoulé depuis la dernière frame en secondes
			float deltaTime = (currentTime - lastFrameTime) / 1000.0f;
			// Mettre à jour le temps de la dernière frame
			lastFrameTime = currentTime;

			// Traiter les événements (clics, touches clavier, etc.)
			handleEvents();
			// Mettre à jour la logique du jeu avec le temps écoulé
			update(deltaTime);
			// Afficher (rendu) le jeu
			render();

			// Limiter la fréquence d'images (FPS) pour ne pas utiliser 100% du CPU
			SDL_Delay(FRAME_DELAY);
		}
	}

	// Fonction qui traite tous les événements SDL (clics, touches, etc.)
	void Game::handleEvents()
	{
		SDL_Event event; // Structure pour stocker un événement SDL
		// Boucle qui traite tous les événements en attente
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			// L'utilisateur ferme la fenêtre
			case SDL_QUIT:
				running = false;
				break;

			// Une touche du clavier est appuyée
			case SDL_KEYDOWN:
				handleKeyPress(event.key.keysym.sym);
				break;

			// Un bouton de la souris est cliqué
			case SDL_MOUSEBUTTONDOWN:
				handleMouseClick(event.button.x, event.button.y);
				break;

			// Un événement de fenêtre (redimensionnement, etc.)
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					// Mettre à jour les dimensions de l'écran si la fenêtre est redimensionnée
					screenWidth = event.window.data1;
					screenHeight = event.window.data2;
				}
				break;
			}
		}
	}

	// Fonction de rendu qui affiche tous les éléments du jeu selon l'état actuel
	void Game::render()
	{
		// Effacer l'écran avec une couleur noire
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Afficher le background dynamique si disponible
		if (backgroundTexture)
		{
			SDL_Rect bgRect = {0, 0, screenWidth, screenHeight};
			SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
		}

		// Afficher différentes choses selon l'état actuel du jeu
		switch (currentState)
		{
		// Si le jeu est en cours, afficher les éléments du gameplay
		case GameState::GAMEPLAY:
			renderGameplay();
			break;

		// Si on est au menu principal
		case GameState::MAIN_MENU:
			// Afficher le menu principal
			renderText("S I 3 L N", screenWidth / 2 - 100, 100, fontLarge, Colors::CYAN);
			renderText("Press ENTER to start", screenWidth / 2 - 150, 300, font, Colors::WHITE);
			renderText("Press ESC to quit", screenWidth / 2 - 120, 350, font, Colors::WHITE);
			renderText("Press F11 to change window mode", screenWidth / 2 - 200, 420, font, Colors::WHITE);
			// Afficher le mode actuel
			{
				std::string modeStr = (windowMode == WindowMode::WINDOWED) ? "Windowed" : (windowMode == WindowMode::BORDERLESS ? "Borderless" : "Fullscreen");
				renderText(std::string("Mode: ") + modeStr, screenWidth / 2 - 80, 460, font, Colors::CYAN);
			}
			break;

        // Affichage du menu pause
		case GameState::PAUSE: {
			// Afficher le gameplay "gelé" en fond
			renderGameplay();
			// Overlay semi-transparent
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
			SDL_Rect overlay = {0, 0, screenWidth, screenHeight};
			SDL_RenderFillRect(renderer, &overlay);
			// Texte PAUSE
			renderText("PAUSE", screenWidth / 2 - 100, screenHeight / 2 - 80, fontLarge, Colors::CYAN);
			renderText("Appuyez sur ESC pour reprendre", screenWidth / 2 - 200, screenHeight / 2, font, Colors::WHITE);
			renderText("ou Q pour quitter", screenWidth / 2 - 120, screenHeight / 2 + 50, font, Colors::WHITE);
			break;
		}

		// Si le jeu est terminé
		case GameState::GAME_OVER:
			renderText("GAME OVER", screenWidth / 2 - 150, 200, fontLarge, Colors::RED);
			renderText("Score: " + std::to_string(currentScore), screenWidth / 2 - 100, 300, font, Colors::WHITE);
			renderText("Press ESC to quit", screenWidth / 2 - 120, 400, font, Colors::WHITE);
			break;

		default:
			break;
		}

		// Mettre à jour le rendu (afficher le buffer à l'écran)
		SDL_RenderPresent(renderer);
	}

	// Fonction qui affiche les éléments du gameplay (joueur, ennemis, projectiles, etc.)
	void Game::renderGameplay()
	{
		// Dessiner l'arrière-plan si la texture existe
		if (backgroundTexture)
		{
			// Créer un rectangle pour couvrir toute la fenêtre
			SDL_Rect bgRect = {0, 0, screenWidth, screenHeight};
			// Copier la texture de l'arrière-plan dans le rendu
			SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
		}

		// Afficher le joueur s'il existe
		if (player)
		{
			// Appeler la fonction de rendu du joueur
			player->render(renderer);
		}

		// Afficher les ennemis
		for (auto &enemy : enemies)
		{
			// Appeler la fonction de rendu de chaque ennemi
			enemy->render(renderer);
		}

		// Afficher les projectiles du joueur
		for (auto &bullet : playerBullets)
		{
			// Appeler la fonction de rendu de chaque projectile du joueur
			bullet->render(renderer);
		}

		// Afficher les projectiles des ennemis
		for (auto &bullet : enemyBullets)
		{
			// Appeler la fonction de rendu de chaque projectile d'ennemi
			bullet->render(renderer);
		}

		// Afficher l'interface utilisateur (HUD)
		renderText("Score: " + std::to_string(currentScore), 10, 10, font, Colors::WHITE);
		renderText("Lives: " + std::to_string(lives), 10, 50, font, Colors::WHITE);
		renderText("Level: " + std::to_string(currentLevel), 10, 90, font, Colors::WHITE);

		// Afficher un texte d'introduction au début du niveau
		if (showLevelIntro && fontLarge)
		{
			std::string intro = currentWorld + " - Level " + std::to_string(currentLevel);
			int textW = 0, textH = 0;
			if (TTF_SizeText(fontLarge, intro.c_str(), &textW, &textH) == 0)
			{
				renderText(intro, screenWidth / 2 - textW / 2, screenHeight / 2 - textH / 2, fontLarge, Colors::CYAN);
			}
			else
			{
				renderText(intro, screenWidth / 2 - 100, screenHeight / 2 - 20, fontLarge, Colors::CYAN);
			}
		}
	}

	// Fonction qui affiche du texte à l'écran avec une police et une couleur spécifiées
	void Game::renderText(const std::string &text, int x, int y, TTF_Font *font, const Color &color)
	{
		// Vérifier que la police existe avant de l'utiliser
		if (!font)
			return;

		// Créer une surface SDL contenant le texte rendu avec la police et la couleur
		SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color.toSDL());
		// Vérifier que la surface a pu être créée
		if (!surface)
			return;

		// Créer une texture à partir de la surface pour pouvoir l'afficher
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
		// Vérifier que la texture a pu être créée
		if (!texture)
		{
			// Si la texture n'a pas pu être créée, libérer la surface et sortir
			SDL_FreeSurface(surface);
			return;
		}

		// Créer un rectangle de destination pour définir où et quelle taille afficher le texte
		SDL_Rect destRect = {x, y, surface->w, surface->h};
		// Copier la texture du texte dans le rendu à la position spécifiée
		SDL_RenderCopy(renderer, texture, nullptr, &destRect);

		// Libérer les ressources allouées
		SDL_DestroyTexture(texture); // Libérer la texture
		SDL_FreeSurface(surface);	 // Libérer la surface
	}

	// Fonction qui change l'état actuel du jeu
	void Game::changeState(GameState newState)
	{
		// Log du changement d'état pour le debug
		std::cout << "Changement d'état: " << static_cast<int>(currentState) << " -> " << static_cast<int>(newState) << std::endl;
		currentState = newState;
	}

	// Fermer l'espace de noms SI3LN
} // namespace SI3LN
