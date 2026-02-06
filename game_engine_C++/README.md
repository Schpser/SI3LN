# SI3LN - Space Invaders III Last Night (C++ Version)

Port C++ du jeu Space Invaders Python/Pygame utilisant SDL2.

## 🎮 À propos

Ce projet est une conversion C++ moderne du jeu SI3LN, un Space Invaders amélioré avec :
- Multiple mondes thématiques
- Système de progression par niveaux
- Power-ups et bonus
- Animations et effets visuels
- Système de score et authentification

## 📋 Prérequis

### Bibliothèques requises
- **SDL2** - Bibliothèque graphique principale
- **SDL2_image** - Chargement d'images (PNG, JPG)
- **SDL2_ttf** - Rendu de texte avec polices TrueType
- **SDL2_mixer** - Audio et musique
- **CMake** (>= 3.15) - Système de build
- **Compilateur C++17** (GCC, Clang, MSVC)

### Installation des dépendances

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y \
    cmake \
    g++ \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-ttf-dev \
    libsdl2-mixer-dev
```

#### macOS (avec Homebrew)
```bash
brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

#### Windows
Téléchargez les bibliothèques depuis :
- https://www.libsdl.org/download-2.0.php
- https://www.libsdl.org/projects/SDL_image/
- https://www.libsdl.org/projects/SDL_ttf/
- https://www.libsdl.org/projects/SDL_mixer/

## 🔨 Compilation

### Linux/macOS
```bash
# Créer le dossier de build
mkdir build && cd build

# Configurer avec CMake
cmake ..

# Compiler
make -j$(nproc)

# Lancer le jeu
./SI3LN
```

### Script de build rapide
```bash
chmod +x build.sh
./build.sh
```

## 🎯 Commandes du jeu

### Menu principal
- **ENTER** : Démarrer le jeu
- **ESC** : Quitter

### En jeu
- **Flèches directionnelles** ou **WASD** : Déplacer le vaisseau
- **ESPACE** : Tirer
- **ESC** : Pause / Retour au menu
- **F11** : Basculer plein écran

## 📁 Structure du projet

```
SI3LN_CPP/
├── CMakeLists.txt          # Configuration CMake
├── README.md               # Ce fichier
├── build.sh               # Script de compilation rapide
├── include/               # Headers (.h)
│   ├── Constants.h        # Constantes du jeu
│   ├── Vector2D.h         # Classe de vecteur 2D
│   ├── Entity.h           # Classe de base des entités
│   ├── Player.h           # Joueur
│   ├── Enemy.h            # Ennemis
│   ├── Bullet.h           # Projectiles
│   └── Game.h             # Classe principale du jeu
├── src/                   # Implémentations (.cpp)
│   ├── main.cpp           # Point d'entrée
│   ├── Game.cpp           # Logique principale
│   ├── Player.cpp         # Logique du joueur
│   ├── Enemy.cpp          # Logique des ennemis
│   ├── Bullet.cpp         # Logique des projectiles
│   └── UI/                # Composants d'interface
└── assets/                # Ressources (images, sons, fonts)
    ├── players/           # Sprites des joueurs
    ├── enemies/           # Sprites des ennemis
    ├── worlds/            # Arrière-plans
    └── fonts/             # Polices de caractères
```

## 🚀 Fonctionnalités implémentées

### Version actuelle (v0.1)
- ✅ Boucle de jeu principale
- ✅ Système d'entités (Player, Enemy, Bullet)
- ✅ Physique de base et mouvement
- ✅ Système de collision
- ✅ Spawn d'ennemis par grille
- ✅ Tir du joueur et des ennemis
- ✅ Système de vies
- ✅ Score
- ✅ Niveaux progressifs
- ✅ Menu principal simple
- ✅ Game Over screen

### À venir
- ⏳ Système d'authentification
- ⏳ Sélection de monde
- ⏳ Sélection de niveau
- ⏳ Power-ups et bonus
- ⏳ Attaques spéciales
- ⏳ Animations avancées
- ⏳ Effets sonores et musique
- ⏳ Sauvegarde des scores
- ⏳ Interface utilisateur complète
- ⏳ Boss de fin de niveau

## 🔄 Migration depuis Python

Le code Python original a été converti en C++ moderne avec les équivalences suivantes :

| Python/Pygame | C++/SDL2 |
|---------------|----------|
| `pygame.sprite.Sprite` | `Entity` class |
| `pygame.Rect` | `SDL_Rect` |
| `pygame.Vector2` | `Vector2D` struct |
| `pygame.sprite.Group` | `std::vector<std::shared_ptr<>>` |
| `pygame.image.load()` | `IMG_LoadTexture()` |
| `pygame.font.Font` | `TTF_Font*` |

## 🐛 Debugging

Pour activer les messages de debug lors de la compilation :
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## 📝 Notes de développement

- Le projet utilise C++17 avec smart pointers (`std::shared_ptr`, `std::unique_ptr`)
- La gestion mémoire est automatique (RAII)
- Le code suit les conventions modernes C++
- Architecture orientée objet avec héritage et polymorphisme

## 🤝 Contribution

Ce projet est une conversion du jeu Python original SI3LN.

## 📄 Licence

Projet éducatif - Voir le fichier LICENSE pour plus de détails.

## 👥 Auteurs

- Version Python : Schpser
- Version C++ : Conversion par Claude

---

**Amusez-vous bien ! 🎮**
