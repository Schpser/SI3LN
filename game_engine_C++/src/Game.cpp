// Inclusion du fichier d'en-tête principal pour la classe Game
#include "Game.h"
// Inclusion pour l'entrée/sortie standard (affichage dans la console)
#include <iostream>
// Inclusion pour les algorithmes (erase-remove idiom)
#include <algorithm>

// Ouverture de l'espace de noms SI3LN
namespace SI3LN {

// Constructeur par défaut de la classe Game qui initialise les membres
Game::Game()
    : window(nullptr) // Pointeur de la fenêtre SDL
    , renderer(nullptr) // Pointeur du moteur de rendu SDL
    , currentState(GameState::MAIN_MENU) // État initial du jeu (menu principal)
    , running(false) // Booléen indiquant si le jeu est en cours d'exécution
    , fullscreen(false) // Booléen pour le mode plein écran
    , screenWidth(DEFAULT_SCREEN_WIDTH) // Largeur de l'écran
    , screenHeight(DEFAULT_SCREEN_HEIGHT) // Hauteur de l'écran
    , lastFrameTime(0) // Temps de la dernière image (frame)
    , currentScore(0) // Score actuel du joueur
    , currentLevel(1) // Niveau actuel du jeu
    , currentWorld("Space") // Monde actuel du jeu
    , lives(MAX_LIVES) // Nombre de vies disponibles
    , playerTexture(nullptr) // Texture du joueur
    , enemyTexture(nullptr) // Texture des ennemis
    , backgroundTexture(nullptr) // Texture de l'arrière-plan
    , font(nullptr) // Police de caractères de taille normale
    , fontLarge(nullptr) // Police de caractères de grande taille
{
}

// Destructeur qui appelle cleanup pour libérer les ressources
Game::~Game() {
    cleanup();
}

bool Game::init() {
    // Initialiser SDL avec le support vidéo et audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialiser SDL_image pour charger les images PNG et JPG
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image initialization failed: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Initialiser SDL_ttf pour le rendu du texte avec les polices TTF
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return false;
    }
    
    // Créer la fenêtre principale du jeu avec les paramètres spécifiés
    window = SDL_CreateWindow(
        "S I 3 L N - Space Invaders III Last Night",
        SDL_WINDOWPOS_CENTERED, // Centrer la fenêtre horizontalement
        SDL_WINDOWPOS_CENTERED, // Centrer la fenêtre verticalement
        screenWidth, // Largeur de la fenêtre
        screenHeight, // Hauteur de la fenêtre
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE // Afficher la fenêtre et la rendre redimensionnable
    );
    
    // Vérifier si la création de la fenêtre a échoué
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Créer le moteur de rendu avec accélération matérielle et synchronisation verticale
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Activer le blending (fusion) pour la transparence des éléments
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Charger les ressources (textures, polices)
    loadAssets();
    
    std::cout << "Game initialized successfully!" << std::endl;
    return true;
}

void Game::loadAssets() {
    std::cout << "Loading assets..." << std::endl;
    
    // Charger les polices de caractères pour le texte du jeu
    font = TTF_OpenFont("assets/fonts/arial.ttf", FONT_SIZE_MEDIUM); // Police de taille moyenne
    fontLarge = TTF_OpenFont("assets/fonts/arial.ttf", FONT_SIZE_LARGE); // Police de grande taille
    
    // Afficher un avertissement si les polices ne peuvent pas être chargées
    if (!font || !fontLarge) {
        std::cerr << "Warning: Could not load fonts. Using system default." << std::endl;
    }
    
    // Charger les textures des entités du jeu (chemins temporaires - à mettre à jour)
    playerTexture = loadTexture("assets/players/player_1/AnimateDiff_00001.001.png"); // Texture du joueur
    enemyTexture = loadTexture("assets/enemies/Apocalyptic_world/boss_enemy.png"); // Texture des ennemis
    backgroundTexture = loadTexture("assets/worlds/background_apocalyptic.jpg"); // Texture de l'arrière-plan
    
    std::cout << "Assets loaded!" << std::endl;
}

// Fonction qui charge une texture à partir d'un chemin d'accès
SDL_Texture* Game::loadTexture(const std::string& path) {
    // Charger la texture avec SDL_image
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    // Vérifier si le chargement a échoué et afficher un message d'erreur
    if (!texture) {
        std::cerr << "Warning: Failed to load texture: " << path << " - " << IMG_GetError() << std::endl;
    }
    // Retourner la texture (peut être nullptr si elle n'a pas pu être chargée)
    return texture;
}

// Fonction de nettoyage qui libère toutes les ressources allouées
void Game::cleanup() {
    // Détruire les textures chargées
    if (playerTexture) SDL_DestroyTexture(playerTexture); // Détruire la texture du joueur
    if (enemyTexture) SDL_DestroyTexture(enemyTexture); // Détruire la texture des ennemis
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture); // Détruire la texture de l'arrière-plan
    
    // Fermer les polices de caractères
    if (font) TTF_CloseFont(font); // Fermer la police normale
    if (fontLarge) TTF_CloseFont(fontLarge); // Fermer la grande police
    
    // Détruire le moteur de rendu et la fenêtre SDL
    if (renderer) SDL_DestroyRenderer(renderer); // Libérer le moteur de rendu
    if (window) SDL_DestroyWindow(window); // Libérer la fenêtre
    
    // Arrêter les sous-systèmes SDL
    TTF_Quit(); // Arrêter SDL_ttf
    IMG_Quit(); // Arrêter SDL_image
    SDL_Quit(); // Arrêter SDL
    
    std::cout << "Game cleaned up successfully!" << std::endl;
}

// Fonction principale qui lance la boucle de jeu
void Game::run() {
    // Initialiser le jeu et vérifier que tout s'est bien passé
    if (!init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return;
    }
    
    // Marquer le jeu comme en cours d'exécution
    running = true;
    // Enregistrer le temps du premier frame
    lastFrameTime = SDL_GetTicks();
    
    // Boucle principale du jeu qui s'exécute tant que running est vrai
    while (running) {
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
void Game::handleEvents() {
    SDL_Event event; // Structure pour stocker un événement SDL
    // Boucle qui traite tous les événements en attente
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
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
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    // Mettre à jour les dimensions de l'écran si la fenêtre est redimensionnée
                    screenWidth = event.window.data1;
                    screenHeight = event.window.data2;
                }
                break;
        }
    }
}

// Fonction qui traite les touches de clavier appuyées
void Game::handleKeyPress(SDL_Keycode key) {
    // Analyser la touche appuyée
    switch (key) {
        // Touche ECHAP : pause ou quitter
        case SDLK_ESCAPE:
            if (currentState == GameState::GAMEPLAY) {
                // Si le jeu est en cours, mettre en pause
                changeState(GameState::PAUSE);
            } else {
                // Sinon, quitter le jeu
                running = false;
            }
            break;
            
        // Touche F11 : basculer le mode plein écran
        case SDLK_F11:
            fullscreen = !fullscreen;
            if (fullscreen) {
                // Passer en mode plein écran
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            } else {
                // Revenir au mode fenêtré
                SDL_SetWindowFullscreen(window, 0);
            }
            break;
            
        // Barre d'espace : tirer (si le jeu est en cours et qu'on n'a pas atteint le nombre max de projectiles)
        case SDLK_SPACE:
            if (currentState == GameState::GAMEPLAY && player && playerBullets.size() < MAX_PLAYER_BULLETS) {
                // Créer un projectile du joueur si possible
                if (player->canShoot()) {
                    auto bullet = std::make_shared<Bullet>(
                        player->getPosition().x, // Position X du joueur
                        player->getPosition().y - player->getHeight() / 2, // Position Y au-dessus du joueur
                        true, // true = projectile du joueur (false pour les ennemis)
                        screenHeight, // Hauteur de l'écran pour les limites
                        Colors::CYAN // Couleur du projectile (cyan)
                    );
                    playerBullets.push_back(bullet);
                    player->resetShootCooldown(); // Réinitialiser le temps avant le prochain tir
                }
            }
            break;
            
        // Touche ENTRÉE : commencer le jeu (depuis le menu)
        case SDLK_RETURN:
            if (currentState == GameState::MAIN_MENU) {
                // Passer à l'état de gameplay
                changeState(GameState::GAMEPLAY);
                // Spawner les ennemis pour le premier niveau
                spawnEnemies();
                // Créer le joueur au centre-bas de l'écran
                player = std::make_unique<Player>(
                    screenWidth / 2.0f, // Position X (centre)
                    screenHeight - 100.0f, // Position Y (bas de l'écran)
                    playerTexture, // Texture du joueur
                    screenWidth, // Largeur de l'écran
                    screenHeight // Hauteur de l'écran
                );
            }
            break;
    }
}

// Fonction qui traite les clics de la souris
void Game::handleMouseClick(int x, int y) {
    // Gérer les clics de souris selon l'état actuel du jeu
    // Les coordonnées x et y sont les positions du clic en pixels
    // TODO: Implémenter la gestion des boutons UI (menus, etc.)
}

// Fonction de mise à jour générale qui délègue selon l'état du jeu
void Game::update(float deltaTime) {
    // Déléguer la mise à jour selon l'état actuel du jeu
    switch (currentState) {
        // Si le jeu est en cours, faire une mise à jour du gameplay
        case GameState::GAMEPLAY:
            updateGameplay(deltaTime);
            break;
            
        // TODO: Ajouter les autres mises à jour d'état (menu, pause, game over)
        default:
            break;
    }
}

// Fonction de mise à jour spécifique au gameplay
void Game::updateGameplay(float deltaTime) {
    // Mettre à jour le joueur
    if (player) {
        // Récupérer l'état du clavier
        const uint8_t* keyState = SDL_GetKeyboardState(nullptr);
        // Laisser le joueur traiter l'entrée clavier
        player->handleInput(keyState);
        // Mettre à jour la position du joueur selon son état
        player->update(deltaTime);
    }
    
    // Mettre à jour les ennemis
    for (auto& enemy : enemies) {
        // Mettre à jour la position et l'état de chaque ennemi
        enemy->update(deltaTime);
        
        // Tir des ennemis (si l'ennemi peut tirer)
        if (enemy->canShoot()) {
            auto bullet = std::make_shared<Bullet>(
                enemy->getPosition().x, // Position X de l'ennemi
                enemy->getPosition().y + enemy->getHeight() / 2, // Position Y sous l'ennemi
                false, // false = projectile d'ennemi (true pour les projectiles du joueur)
                screenHeight, // Hauteur de l'écran pour les limites
                Colors::RED // Couleur du projectile (rouge)
            );
            enemyBullets.push_back(bullet);
        }
    }
    
    // Mettre à jour les projectiles du joueur
    for (auto& bullet : playerBullets) {
        // Mettre à jour la position de chaque projectile
        bullet->update(deltaTime);
    }
    
    // Mettre à jour les projectiles des ennemis
    for (auto& bullet : enemyBullets) {
        // Mettre à jour la position de chaque projectile
        bullet->update(deltaTime);
    }
    
    // Supprimer les projectiles du joueur qui ne sont plus actifs (sortis de l'écran)
    playerBullets.erase(
        std::remove_if(playerBullets.begin(), playerBullets.end(),
            [](const auto& b) { return !b->isAlive(); }), // Garder uniquement les projectiles vivants
        playerBullets.end()
    );
    
    // Supprimer les projectiles des ennemis qui ne sont plus actifs
    enemyBullets.erase(
        std::remove_if(enemyBullets.begin(), enemyBullets.end(),
            [](const auto& b) { return !b->isAlive(); }), // Garder uniquement les projectiles vivants
        enemyBullets.end()
    );
    
    // Vérifier les collisions entre les projectiles et les entités
    checkCollisions();
    
    // Vérifier la condition de victoire du niveau (tous les ennemis éliminés)
    if (enemies.empty()) {
        std::cout << "Level complete!" << std::endl;
        // Passer au niveau suivant
        currentLevel++;
        // Spawner les ennemis pour le nouveau niveau
        spawnEnemies();
    }
    
    // Vérifier la condition d'échec (le joueur n'a plus de vies)
    if (lives <= 0) {
        // Passer à l'écran de fin de jeu
        changeState(GameState::GAME_OVER);
    }
}

// Fonction qui vérifie les collisions entre les projectiles et les entités
void Game::checkCollisions() {
    // Vérifier les collisions : Projectiles du joueur vs ennemis
    for (auto bulletIt = playerBullets.begin(); bulletIt != playerBullets.end();) {
        bool bulletHit = false; // Marquer si le projectile a touché quelque chose
        
        // Vérifier chaque projectile contre chaque ennemi
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end();) {
            // Tester si le projectile intersecte (touche) l'ennemi
            if ((*bulletIt)->intersects(**enemyIt)) {
                // Ajouter au score quand un ennemi est touché
                currentScore += 10 * currentLevel;
                // Marquer le projectile comme mort (l'enlever après)
                (*bulletIt)->kill();
                // Supprimer l'ennemi qui a été touché
                enemyIt = enemies.erase(enemyIt);
                // Marquer que le projectile a touché quelque chose
                bulletHit = true;
                break; // Arrêter la boucle d'ennemis (un projectile = un ennemi)
            } else {
                ++enemyIt; // Passer à l'ennemi suivant
            }
        }
        
        // Supprimer le projectile s'il a touché quelque chose ou s'il est mort
        if (bulletHit || !(*bulletIt)->isAlive()) {
            bulletIt = playerBullets.erase(bulletIt); // Enlever le projectile et obtenir l'itérateur suivant
        } else {
            ++bulletIt; // Passer au projectile suivant
        }
    }
    
    // Vérifier les collisions : Projectiles des ennemis vs joueur
    if (player) {
        // Vérifier chaque projectile d'ennemi contre le joueur
        for (auto bulletIt = enemyBullets.begin(); bulletIt != enemyBullets.end();) {
            // Tester si le projectile intersecte (touche) le joueur
            if ((*bulletIt)->intersects(*player)) {
                // Réduire le nombre de vies du joueur
                lives--;
                // Supprimer le projectile qui a touché le joueur
                bulletIt = enemyBullets.erase(bulletIt);
                std::cout << "Player hit! Lives: " << lives << std::endl;
            } else {
                ++bulletIt; // Passer au projectile suivant
            }
        }
    }
}

// Fonction qui crée (spawne) les ennemis pour le niveau actuel
void Game::spawnEnemies() {
    // Vider la liste des ennemis de l'ancien niveau
    enemies.clear();
    
    // Calculer le nombre de lignes et colonnes d'ennemis en fonction du niveau actuel
    // Plus le niveau augmente, plus il y a d'ennemis (avec une limite max)
    int rows = std::min(ENEMY_SPAWN_BASE_ROWS + currentLevel / 2, ENEMY_SPAWN_MAX_ROWS);
    int cols = std::min(ENEMY_SPAWN_BASE_COLS + currentLevel / 2, ENEMY_SPAWN_MAX_COLS);
    
    // Calculer la largeur totale occupée par les ennemis
    int totalWidth = cols * (ENEMY_SIZE + ENEMY_SPACING_X);
    // Calculer la position X de départ pour centrer les ennemis
    int startX = (screenWidth - totalWidth) / 2;
    // Position Y pour le haut de la grille d'ennemis
    int startY = 80;
    
    // Créer les ennemis en grille (rows x cols)
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // Calculer la position X de l'ennemi (centrer chaque ennemi dans sa cellule)
            float x = startX + col * (ENEMY_SIZE + ENEMY_SPACING_X) + ENEMY_SIZE / 2;
            // Calculer la position Y de l'ennemi (centrer chaque ennemi dans sa cellule)
            float y = startY + row * (ENEMY_SIZE + ENEMY_SPACING_Y) + ENEMY_SIZE / 2;
            
            // Créer un nouvel ennemi et l'ajouter à la liste
            auto enemy = std::make_shared<Enemy>(x, y, enemyTexture, screenWidth, currentLevel);
            enemies.push_back(enemy);
        }
    }
    
}

// Fonction de rendu qui affiche tous les éléments du jeu selon l'état actuel
void Game::render() {
    // Effacer l'écran avec une couleur noire (RGB: 0,0,0 avec alpha: 255)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Afficher différentes choses selon l'état actuel du jeu
    switch (currentState) {
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
            break;
            
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
void Game::renderGameplay() {
    // Dessiner l'arrière-plan si la texture existe
    if (backgroundTexture) {
        // Créer un rectangle pour couvrir toute la fenêtre
        SDL_Rect bgRect = {0, 0, screenWidth, screenHeight};
        // Copier la texture de l'arrière-plan dans le rendu
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
    }
    
    // Afficher le joueur s'il existe
    if (player) {
        // Appeler la fonction de rendu du joueur
        player->render(renderer);
    }
    
    // Afficher les ennemis
    for (auto& enemy : enemies) {
        // Appeler la fonction de rendu de chaque ennemi
        enemy->render(renderer);
    }
    
    // Afficher les projectiles du joueur
    for (auto& bullet : playerBullets) {
        // Appeler la fonction de rendu de chaque projectile du joueur
        bullet->render(renderer);
    }
    
    // Afficher les projectiles des ennemis
    for (auto& bullet : enemyBullets) {
        // Appeler la fonction de rendu de chaque projectile d'ennemi
        bullet->render(renderer);
    }
    
    // Afficher l'interface utilisateur (HUD)
    renderText("Score: " + std::to_string(currentScore), 10, 10, font, Colors::WHITE);
    renderText("Lives: " + std::to_string(lives), 10, 50, font, Colors::WHITE);
    renderText("Level: " + std::to_string(currentLevel), 10, 90, font, Colors::WHITE);
}

// Fonction qui affiche du texte à l'écran avec une police et une couleur spécifiées
void Game::renderText(const std::string& text, int x, int y, TTF_Font* font, const Color& color) {
    // Vérifier que la police existe avant de l'utiliser
    if (!font) return;
    
    // Créer une surface SDL contenant le texte rendu avec la police et la couleur
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color.toSDL());
    // Vérifier que la surface a pu être créée
    if (!surface) return;
    
    // Créer une texture à partir de la surface pour pouvoir l'afficher
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    // Vérifier que la texture a pu être créée
    if (!texture) {
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
    SDL_FreeSurface(surface); // Libérer la surface
}

// Fonction qui change l'état actuel du jeu
void Game::changeState(GameState newState) {
    // Mettre à jour l'état actuel avec le nouvel état
    currentState = newState;
}

// Fermer l'espace de noms SI3LN
} // namespace SI3LN
