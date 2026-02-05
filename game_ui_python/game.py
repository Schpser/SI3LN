# Classe principale du jeu SI3LN
# Intègre tous les écrans et la logique du jeu
"""
Classe principale du jeu SI3LN
Intègre tous les écrans et la logique du jeu
"""


import pygame  # Bibliothèque pour l'affichage et le jeu
import random  # Pour les nombres aléatoires
import platform  # Pour détecter le système d'exploitation
import sys  # Pour quitter le programme
import os  # Pour la gestion des fichiers et dossiers
import subprocess  # Pour lancer le jeu C++
from constants import *  # Importation des constantes du jeu
from utils.logger import setup_logging, get_logger, info, warning, error, debug
from utils.game_utils import load_image, create_bullet_surface
from ui_components import Button, InputField, Panel, ImageButton, AnimatedPlayer, PopUp
from profile import ProfileScreen
from level_selector import LevelSelector
from managers.collision_manager import CollisionManager
from managers.entity_manager import EntityManager
from managers.game_state import GameState
from entities import Player, Bullet, SpecialAttack

# Variables globales pour le moteur C++

# Fonction pour trouver dynamiquement l'exécutable du jeu C++
def find_cpp_game_exe():
    """
    Recherche l'exécutable du jeu C++ dans les emplacements courants.
    Retourne (chemin_exe, dossier_exe) ou (None, None) si non trouvé.
    """
    exe_name = "SI3LN.exe" if platform.system() == "Windows" else "SI3LN"  # Nom selon l'OS
    search_dirs = [
        os.path.join(os.path.dirname(__file__), "..", "game_engine_C++", "build"),  # Dossier parent (SI3LN/game_engine_C++/build)
        os.path.join(os.getcwd(), "..", "game_engine_C++", "build"),  # Depuis game_ui_python, remonter d'un niveau
        os.path.join(os.getcwd(), "game_engine_C++", "build"),  # Dossier build local
        os.path.expanduser("~/Holbeton/SI3LN/game_engine_C++/build"),  # Chemin absolu Holbeton
        os.path.expanduser("~/SI3LN/game_engine_C++/build"),  # Dossier home
        os.path.expanduser("~/game_engine_C++/build"),
        os.path.join(os.getcwd(), "build"),
    ]
    for d in search_dirs:
        exe_path = os.path.join(d, exe_name)
        if os.path.isfile(exe_path):
            return exe_path, d
    return None, None

CPP_GAME_EXE, CPP_GAME_DIR = find_cpp_game_exe()
if not CPP_GAME_EXE:
    # Fallback valeurs par défaut si non trouvé
    CPP_GAME_EXE = "SI3LN"
    CPP_GAME_DIR = "game_engine_C++/build"
USE_CPP_ENGINE = True  # Mettre à True pour activer le moteur C++
from auth import AuthSystem
from scores import ScoreManager
from constants import STATE_MAIN_MENU
from level_selector import LevelSelector
from managers.collision_manager import CollisionManager
from managers.entity_manager import EntityManager
from constants import DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, MAX_LIVES, SHIELD_DURATION, MEGA_SHOT_DURATION, WORLDS, BULLET_SIZE_PLAYER, BULLET_SIZE_ENEMY, CYAN, RED, GREEN, ORANGE, WHITE, YELLOW, BLUE, PURPLE, LIGHT_BLUE, SAND_COLOR, BROWN, PROFILE_ICON_SIZE, PROFILE_ICON_POSITION, MESSAGE_DISPLAY_DURATION
from constants import STATE_LOGIN, STATE_REGISTER, STATE_GAMEPLAY, STATE_PAUSE, STATE_GAME_OVER, STATE_LEVEL_WIN, STATE_LEVEL_SELECT
from constants import ANIMATION_SIZE_PREVIEW, ANIMATION_SIZE_CHARACTER_SELECT, PLAYER_START_Y_OFFSET
from constants import FONT_SIZE_LARGE, FONT_SIZE_MEDIUM, FONT_SIZE_SMALL, FONT_SIZE_TINY
from constants import DEFAULT_FALLBACK_COLOR
from level_selector import LevelSelector
from managers.collision_manager import CollisionManager
from managers.entity_manager import EntityManager
from scores import ScoreManager
from auth import AuthSystem
from ui_components import PopUp
from constants import *
from utils.logger import *
from utils.game_utils import *
from ui_components import *


# Fonction pour trouver dynamiquement l'exécutable du jeu C++
def find_cpp_game_exe():
    """
    Recherche l'exécutable du jeu C++ dans les emplacements courants.
    Retourne (chemin_exe, dossier_exe) ou (None, None) si non trouvé.
    """
    exe_name = "SI3LN.exe" if platform.system() == "Windows" else "SI3LN"  # Nom selon l'OS
    search_dirs = [
        os.path.join(os.getcwd(), "game_engine_C++", "build"),  # Dossier build local
        os.path.join(os.path.dirname(__file__), "game_engine_C++", "build"),  # Dossier build relatif au script
        os.path.expanduser("~/SI3LN/game_engine_C++/build"),  # Dossier home
        os.path.expanduser("~/game_engine_C++/build"),
        os.path.join(os.getcwd(), "build"),
        os.path.dirname(os.path.abspath(__file__)),
    ]
    for d in search_dirs:
        exe_path = os.path.join(d, exe_name)
        if os.path.isfile(exe_path):
            return exe_path, d
    return None, None


# Correction: Déplacer le code de chargement des assets dans une méthode de la classe Game
class Game:
    # Flag at class-level to ensure attribute always exists on instances
    launching_cpp = False
    def launch_cpp_game(self):
        # Prevent reentrant launches
        if getattr(self, 'launching_cpp', False):
            debug("C++ launch already in progress")
            return
        self.launching_cpp = True
        try:
            info(f"Launching C++ game engine for world={self.current_world}, level={self.current_level}")
            # Affiche un écran de chargement
            self.screen.fill(BLACK)
            loading_text = self.font_large.render("CHARGEMENT...", True, WHITE)
            loading_rect = loading_text.get_rect(center=(self.screen_width // 2, self.screen_height // 2))
            self.screen.blit(loading_text, loading_rect)
            pygame.display.flip()
            pygame.time.wait(500)

            # Minimise la fenêtre pygame pendant le jeu C++
            pygame.display.iconify()

            # Lance le moteur C++ dans son dossier (pour que les chemins relatifs fonctionnent)
            try:
                # Passer le monde, le niveau et le personnage sélectionné en arguments
                # Passer le mode de fenêtre au binaire C++ (argument optionnel)
                mode_arg = getattr(self, 'selected_window_mode', None)
                args = [CPP_GAME_EXE, self.current_world, str(self.current_level), str(self.selected_character)]
                if mode_arg:
                    args.append(mode_arg)

                info(f"Executing C++: {args} (cwd={CPP_GAME_DIR})")

                # Déterminer l'ordre des drivers à essayer selon la session
                session = os.environ.get("XDG_SESSION_TYPE", "").lower()
                if session == "wayland":
                    drivers = [None, "wayland", "x11"]
                elif session == "x11":
                    drivers = [None, "x11", "wayland"]
                else:
                    drivers = [None, "wayland", "x11"]

                last_result = None
                last_exc = None

                for drv in drivers:
                    env = os.environ.copy()
                    if drv:
                        env["SDL_VIDEODRIVER"] = drv
                    info(f"Attempting C++ launch with SDL_VIDEODRIVER={drv or '<default>'}")
                    try:
                        # Bloque jusqu'à la fin ; capture les sorties pour debug
                        result = subprocess.run(args, cwd=CPP_GAME_DIR, env=env, capture_output=True, text=True)
                        last_result = result
                        info(f"C++ exited with code {result.returncode}")
                        if result.stdout:
                            info("C++ stdout:\n" + result.stdout.strip())
                        if result.stderr:
                            error("C++ stderr:\n" + result.stderr.strip())

                        # Si le binaire retourne 0, on considère que c'est un succès
                        if result.returncode == 0:
                            break
                        # sinon, on essaie le driver suivant pour voir si ça règle le problème
                    except FileNotFoundError as e:
                        error(f"C++ game executable not found: {e}")
                        last_exc = e
                        break
                    except Exception as e:
                        error(f"Error launching C++ game with driver {drv}: {e}")
                        last_exc = e
                        # try next driver

                # Si on a une erreur non nulle ou des sorties, afficher une popup avec les logs
                if last_result and last_result.returncode != 0:
                    content = []
                    if last_result.stdout:
                        content.append("STDOUT:")
                        content.extend(last_result.stdout.strip().splitlines())
                    if last_result.stderr:
                        content.append("STDERR:")
                        # limiter le nombre de lignes pour tenir dans la popup
                        content.extend(last_result.stderr.strip().splitlines()[:20])

                    popup = PopUp(700, 300, "C++ launch failed", content, self.screen_width, self.screen_height, self.font_medium)
                    popup.open()
                    self.launch_popup = popup
                    self.show_message("Erreur: le jeu C++ a échoué (voir popup)", RED)
                elif last_exc:
                    self.show_message(f"Erreur: {last_exc}", RED)
            except Exception as e:
                error(f"C++ inner try block exception: {e}")
                self.show_message(f"Erreur: {e}", RED)

        except Exception as e:
            error(f"Error launching C++ game: {e}")
            self.show_message(f"Erreur: {e}", RED)

        finally:
            # Revient à l'interface Python
            info("Returning to Python UI...")

            # Restaure la fenêtre pygame
            pygame.display.set_mode(
                (self.screen_width, self.screen_height),
                pygame.RESIZABLE
            )

            # Retourne au menu de sélection de niveau
            if hasattr(self, 'level_selector'):
                self.level_selector.open()
            self.state = STATE_LEVEL_SELECT
            self.show_message("Retour au menu!", GREEN)
            # Clear launching flag regardless of result
            self.launching_cpp = False
    def __init__(self):
        # Initialisation du système de logs
        setup_logging()
        self.logger = get_logger()
        info("Initialisation du jeu...")
        # Initialisation de Pygame
        pygame.init()
        # Récupère les infos de l'écran
        self.screen_info = pygame.display.Info()
        # Largeur de la fenêtre
        self.screen_width = DEFAULT_SCREEN_WIDTH
        # Hauteur de la fenêtre
        self.screen_height = DEFAULT_SCREEN_HEIGHT
        # Crée la fenêtre redimensionnable
        self.screen = pygame.display.set_mode(
            (self.screen_width, self.screen_height),
            pygame.RESIZABLE
        )
        # Titre de la fenêtre
        pygame.display.set_caption("S I 3 L N")
        # Horloge pour le framerate
        self.clock = pygame.time.Clock()
        # Booléen pour la boucle principale
        self.running = True
        # Indique si on est en plein écran (par défaut non)
        self.is_fullscreen = False
        # Dernier toggle plein écran (ms) pour debounce
        self.last_fullscreen_toggle_time = 0
        # Gestionnaire d'état du jeu
        self.state_manager = GameState(STATE_MAIN_MENU)
        # État courant du jeu
        self.state = STATE_MAIN_MENU  # Pour compatibilité
        # État précédent
        self.prev_state = None
        # Système d'authentification
        self.auth = AuthSystem()
        # Gestionnaire de scores
        self.score_manager = ScoreManager()
        # Score courant
        self.current_score = 0
        # Niveau courant
        self.current_level = 1
        # Monde courant
        self.current_world = "Space"
        # Nombre de vies
        self.lives = MAX_LIVES
        # Personnage sélectionné
        self.selected_character = 0
        # Groupes de bonus
        self.bonuses = pygame.sprite.Group()
        # Dictionnaire des bonus actifs
        self.active_bonuses = {
            "shield": {"active": False, "timer": 0, "duration": SHIELD_DURATION},
            "mega_shot": {"active": False, "timer": 0, "duration": MEGA_SHOT_DURATION}
        }
        # Groupes d'attaques spéciales
        self.special_attacks = pygame.sprite.Group()
        # Debuffs du joueur
        self.player_debuffs = {
            "frozen": False,
            "blinded": False,
            "rooted": False,
            "timer": 0,
            "duration": 0
        }
        # Chargement des assets
        self.load_assets()
        # Initialisation du message temporaire
        self.message = ""
        self.message_timer = 0
        # Création de l'interface utilisateur (boutons, champs, etc.)
        self.create_ui()
        # Initialisation des gestionnaires de collisions et entités
        self.collision_manager = CollisionManager(self)
        self.entity_manager = EntityManager(self)
        # Initialisation des écrans de profil et de sélection de niveau
        self.profile_screen = ProfileScreen(self.screen, self.auth, self.players)
        self.level_selector = LevelSelector(self.screen, WORLDS)

    def load_assets(self):
        # Chargement du fond du menu principal (doit être fait avant d'utiliser self.menu_bg)
        try:
            self.menu_bg = load_image("worlds/home_page.jpg", (self.screen_width, self.screen_height), False)
        except Exception as e:
            warning(f"Erreur lors du chargement du fond du menu: {e}")
            self.menu_bg = pygame.Surface((self.screen_width, self.screen_height))
            self.menu_bg.fill((30, 30, 60))  # Fallback color

        # Chargement des backgrounds pour chaque monde
        self.world_backgrounds = {}
        for world_key, world_data in WORLDS.items():
            try:
                bg_path = f"worlds/{world_data['background']}"
                self.world_backgrounds[world_key] = load_image(bg_path, (self.screen_width, self.screen_height), False)
            except Exception as e:
                warning(f"Erreur lors du chargement du fond pour {world_key}: {e}")
                self.world_backgrounds[world_key] = self.menu_bg
        # Fond de jeu par défaut (Space)
        self.game_bg = self.world_backgrounds.get("Space", self.menu_bg)
        # Chargement des polices
        try:
            # Utilise une police par défaut si FONT_PATH n'est pas défini
            self.font_large = pygame.font.Font(None, 72)
            self.font_medium = pygame.font.Font(None, 48)
            self.font_small = pygame.font.Font(None, 28)
            self.font_tiny = pygame.font.Font(None, 18)
        except Exception as e:
            warning(f"Erreur lors du chargement des polices: {e}")
            self.font_large = pygame.font.SysFont("Arial", 72)
            self.font_medium = pygame.font.SysFont("Arial", 48)
            self.font_small = pygame.font.SysFont("Arial", 28)
            self.font_tiny = pygame.font.SysFont("Arial", 18)
        # Chargement des portraits des joueurs (pour l'écran de profil)
        self.players = []
        self.players_animation_folders = []
        base_path = "assets/players"
        for i in range(1, 9):
            player_folder = os.path.join(base_path, f"player_{i}")
            portrait_path = os.path.join(player_folder, "portrait.png")
            if os.path.exists(portrait_path):
                try:
                    img = pygame.image.load(portrait_path).convert_alpha()
                    self.players.append(img)
                    self.players_animation_folders.append(player_folder)
                except Exception as e:
                    warning(f"Erreur lors du chargement du portrait du joueur {i}: {e}")
                    self.players.append(pygame.Surface((64, 64)))
                    self.players_animation_folders.append(None)
            else:
                self.players.append(pygame.Surface((64, 64)))
                self.players_animation_folders.append(None)
        # Chargement des sprites de joueurs pour le gameplay
        self.players_gameplay = []
        for i in range(1, 9):
            player_folder = os.path.join(base_path, f"player_{i}")
            gameplay_path = os.path.join(player_folder, "gameplay.png")
            if os.path.exists(gameplay_path):
                try:
                    img = pygame.image.load(gameplay_path).convert_alpha()
                    self.players_gameplay.append(img)
                except Exception as e:
                    warning(f"Erreur lors du chargement du sprite gameplay du joueur {i}: {e}")
                    self.players_gameplay.append(pygame.Surface((64, 64)))
            else:
                self.players_gameplay.append(pygame.Surface((64, 64)))
        # Initialisation de l'icône de profil (None au début)
        self.profile_icon = None
        # Initialisation des groupes d'entités
        self.enemies = pygame.sprite.Group()
        self.player_bullets = pygame.sprite.Group()
        self.enemy_bullets = pygame.sprite.Group()
        self.explosions = pygame.sprite.Group()
        # Initialisation du timer d'attaque spéciale
        self.last_special_attack_time = 0
        self.special_attack_cooldown = 5000  # ms
        # Chargement terminé
        info("Assets chargés avec succès.")
    def __init__(self):
        # Initialisation du système de logs
        setup_logging()
        self.logger = get_logger()
        info("Initialisation du jeu...")
        # Initialisation de Pygame
        pygame.init()
        # Récupère les infos de l'écran
        self.screen_info = pygame.display.Info()
        # Largeur de la fenêtre
        self.screen_width = DEFAULT_SCREEN_WIDTH
        # Hauteur de la fenêtre
        self.screen_height = DEFAULT_SCREEN_HEIGHT
        # Crée la fenêtre redimensionnable
        self.screen = pygame.display.set_mode(
            (self.screen_width, self.screen_height),
            pygame.RESIZABLE
        )
        # Titre de la fenêtre
        pygame.display.set_caption("S I 3 L N")
        # Horloge pour le framerate
        self.clock = pygame.time.Clock()
        # Booléen pour la boucle principale
        self.running = True
        # Indique si on est en plein écran (par défaut non)
        self.is_fullscreen = False
        # Dernier toggle plein écran (ms) pour debounce
        self.last_fullscreen_toggle_time = 0
        # Gestionnaire d'état du jeu
        self.state_manager = GameState(STATE_MAIN_MENU)
        # État courant du jeu
        self.state = STATE_MAIN_MENU  # Pour compatibilité
        # État précédent
        self.prev_state = None
        # Système d'authentification
        self.auth = AuthSystem()
        # Gestionnaire de scores
        self.score_manager = ScoreManager()
        # Score courant
        self.current_score = 0
        # Niveau courant
        self.current_level = 1
        # Monde courant
        self.current_world = "Space"
        # Nombre de vies
        self.lives = MAX_LIVES
        # Personnage sélectionné
        self.selected_character = 0
        # Groupes de bonus
        self.bonuses = pygame.sprite.Group()
        # Dictionnaire des bonus actifs
        self.active_bonuses = {
            "shield": {"active": False, "timer": 0, "duration": SHIELD_DURATION},
            "mega_shot": {"active": False, "timer": 0, "duration": MEGA_SHOT_DURATION}
        }
        # Groupes d'attaques spéciales
        self.special_attacks = pygame.sprite.Group()
        # Debuffs du joueur
        self.player_debuffs = {
            "frozen": False,
            "blinded": False,
            "rooted": False,
            "timer": 0,
            "duration": 0
        }
        # Chargement des assets
        self.load_assets()

        # Initialisation du message temporaire
        self.message = ""
        self.message_timer = 0

        # Création de l'interface utilisateur (boutons, champs, etc.)
        self.create_ui()
        # Initialisation des gestionnaires de collisions et entités
        self.collision_manager = CollisionManager(self)
        self.entity_manager = EntityManager(self)
        # Initialisation des écrans de profil et de sélection de niveau
        self.profile_screen = ProfileScreen(self.screen, self.auth, self.players)
        self.level_selector = LevelSelector(self.screen, WORLDS)
        # Popup de lancement / logs du binaire C++ (remplie si erreur lors du lancement)
        self.launch_popup = None
        # Création de l'UI
        def update_gameplay(self):
            """Met à jour la logique du gameplay"""
            # Met à jour les ennemis
            for enemy in self.enemies:
                enemy.update()
                # Tir des ennemis
                current_time = pygame.time.get_ticks()
                if enemy.can_shoot() and random.random() < enemy.shoot_chance:
                    bullet = Bullet(enemy.rect.centerx,
                                enemy.rect.bottom,
                                self.enemy_bullet_img,
                                False,
                                self.screen_height)
                    self.enemy_bullets.add(bullet)
                    enemy.last_shot = current_time
            # Met à jour les explosions
            self.explosions.update()
            # Met à jour les bonus
            self.bonuses.update()
            # Met à jour les attaques spéciales
            self.special_attacks.update()
            # Gestion des debuffs
            self.update_debuffs()
            # Chance de faire apparaître un bonus
            if random.random() < 0.001:  # 0.1% de chance par frame
                x = random.randint(50, self.screen_width - 50)
                self.spawn_bonus(x, 0)
            # Chance de déclencher une attaque spéciale
            current_time = pygame.time.get_ticks()
            if (current_time - self.last_special_attack_time > self.special_attack_cooldown and 
                random.random() < 0.01):  # 1% de chance quand le cooldown est écoulé
                self.trigger_world_special()
                self.last_special_attack_time = current_time
            # Nettoie les sprites morts pour éviter les fuites mémoire
            self.cleanup_sprites()
            # Détection des collisions
            self.check_collisions()
            debug(f"Before win check - Enemies: {len(self.enemies)}")
            # Vérifie la condition de victoire
            if len(self.enemies) == 0:
                debug("WIN CONDITION TRIGGERED!")
                self.state = STATE_LEVEL_WIN
                if self.auth.current_user:
                    self.auth.update_user_data(
                        high_score=max(self.current_score, 
                                      self.auth.get_user_data("high_score") or 0)
                    )
        base_path = "assets/players"
        info("Loading player portraits (optimized with lazy loading)...")
        start_time = pygame.time.get_ticks()
        for i in range(1, 9):
            player_folder = os.path.join(base_path, f"player_{i}")  # Dossier du joueur
            
            if not os.path.exists(player_folder):
                warning(f"Player folder not found: {player_folder}")
                self.players.append(None)
                self.players_gameplay.append(None)
                self.players_animation_folders.append(None)
                continue
            
            png_files = [f for f in os.listdir(player_folder) if f.lower().endswith(".png")]
            
            if png_files:
                # Charge uniquement la première frame (pour l'affichage rapide menu/profil)
                image_file = os.path.join(player_folder, png_files[0])
                try:
                    image_large = pygame.image.load(image_file)  # Portrait grand format
                    self.players.append(image_large)
                    image_small = pygame.transform.scale(image_large, (PLAYER_PORTRAIT_SIZE, PLAYER_PORTRAIT_SIZE))
                    self.players_gameplay.append(image_small)
                    # Stocke le dossier pour charger les animations plus tard (profil)
                    self.players_animation_folders.append(player_folder)
                except Exception as e:
                    error(f"Could not load player image {image_file}: {e}")
                    self.players.append(None)
                    self.players_gameplay.append(None)
                    self.players_animation_folders.append(None)
            else:
                warning(f"No PNG files found in {player_folder}")
                self.players.append(None)
                self.players_gameplay.append(None)
                self.players_animation_folders.append(None)
        # Temps de chargement des portraits
        elapsed = pygame.time.get_ticks() - start_time
        info(f"Players loaded in {elapsed}ms (lazy loading enabled)")
        # Préchargement des animations pour une sélection fluide
        info("Preloading character animations...")
        preload_start = pygame.time.get_ticks()
        # Précharge les tailles utilisées dans le jeu
        preload_character_animations(ANIMATION_SIZE_PREVIEW[0], ANIMATION_SIZE_PREVIEW[1], max_players=9)
        preload_character_animations(ANIMATION_SIZE_CHARACTER_SELECT[0], ANIMATION_SIZE_CHARACTER_SELECT[1], max_players=9)
        preload_elapsed = pygame.time.get_ticks() - preload_start
        info(f"Animations preloaded in {preload_elapsed}ms - Ready for smooth selection!")
        # Chargement des bullets (projectiles)
        try:
            default_colors = WORLDS["Space"]["bullet_colors"]
            self.player_bullet_img = create_bullet_surface(
                default_colors["player"][0],
                default_colors["player"][1],
                BULLET_SIZE_PLAYER
            )
            self.enemy_bullet_img = create_bullet_surface(
                default_colors["enemy"][0],
                default_colors["enemy"][1],
                BULLET_SIZE_ENEMY
            )
        except Exception as e:
            warning(f"Could not create bullets: {e}")
            # Fallback : projectiles rectangles colorés
            self.player_bullet_img = pygame.Surface(BULLET_SIZE_PLAYER)
            self.player_bullet_img.fill(CYAN)
            self.enemy_bullet_img = pygame.Surface(BULLET_SIZE_ENEMY)
            self.enemy_bullet_img.fill(RED)
        # Chargement des ennemis et boss (avec fallback)
        self.enemy_images = {}
        self.boss_images = {}
        for world_key in WORLDS.keys():
            try:
                self.enemy_images[world_key] = load_enemy_images(world_key, (ENEMY_SIZE, ENEMY_SIZE))
                self.boss_images[world_key] = load_boss_images(world_key, (BOSS_SIZE, BOSS_SIZE))
            except Exception as e:
                warning(f"Could not load enemies for {world_key}: {e}")
                # Fallback : rectangles colorés
                fallback = []
                for _ in range(3):
                    enemy_surf = pygame.Surface((ENEMY_SIZE, ENEMY_SIZE))
                    enemy_surf.fill(FALLBACK_ENEMY_COLOR)
                    fallback.append(enemy_surf)
                self.enemy_images[world_key] = fallback
                self.boss_images[world_key] = fallback
        # Images ennemis courantes (par défaut : Space)
        self.current_enemy_images = self.enemy_images.get("Space", [])
    
    def create_ui(self):
        """Crée tous les éléments d'interface utilisateur (UI)"""
        cx = self.screen_width // 2  # Centre X de l'écran
        cy = self.screen_height // 2  # Centre Y de l'écran
        # Boutons du menu principal (style arcade, fond transparent)
        self.btn_start = Button(cx, cy - 40, 250, 70, "START", self.font_medium, bg_color=None, text_color=WHITE, border_color=WHITE)
        self.btn_continue = Button(cx, cy + 50, 250, 70, "PLAY", self.font_medium, bg_color=None, text_color=WHITE, border_color=WHITE)
        self.btn_help = Button(self.screen_width - 100, self.screen_height - 70, 150, 50, "AIDE", self.font_small, bg_color=None, text_color=WHITE, border_color=WHITE)
        self.btn_game = Button(self.screen_width - 100, self.screen_height - 130, 150, 50, "GAME", self.font_small, bg_color=None, text_color=WHITE, border_color=WHITE)
        self.btn_quit = Button(self.screen_width - 100, self.screen_height - 190, 150, 50, "QUITTER", self.font_small, bg_color=None, text_color=WHITE, border_color=WHITE)
        # Champs et boutons de l'écran de connexion
        self.login_username = InputField(cx - 150, cy - 80, 300, 45, self.font_small, "Pseudo:")
        self.login_password = InputField(cx - 150, cy, 300, 45, self.font_small, "Mot de passe:", password=True)
        self.btn_login = Button(cx, cy + 80, 200, 50, "CONNEXION", self.font_small, bg_color=GREEN)
        self.btn_to_register = Button(cx, cy + 150, 250, 50, "Créer un compte", self.font_small)
        self.btn_guest = Button(cx, cy + 210, 250, 50, "Mode invité", self.font_small, bg_color=ORANGE)
        # Champs et boutons de l'écran d'inscription
        self.register_username = InputField(cx - 150, cy - 120, 300, 45, self.font_small, "Pseudo:")
        self.register_email = InputField(cx - 150, cy - 50, 300, 45, self.font_small, "Email (optionnel):")
        self.register_password = InputField(cx - 150, cy + 20, 300, 45, self.font_small, "Mot de passe:", password=True)
        self.register_confirm = InputField(cx - 150, cy + 90, 300, 45, self.font_small, "Confirmer:", password=True)
        self.btn_register = Button(cx, cy + 170, 200, 50, "S'INSCRIRE", self.font_small, bg_color=GREEN)
        self.btn_back_login = Button(cx, cy + 230, 200, 50, "RETOUR", self.font_small)
        # Boutons de l'écran game over
        self.btn_restart = Button(cx - 130, self.screen_height - 80, 200, 60, "RESTART", self.font_medium, bg_color=ORANGE)
        self.btn_finish = Button(cx + 130, self.screen_height - 80, 200, 60, "FINISH", self.font_medium, bg_color=RED)
        # Boutons de l'écran victoire de niveau
        self.btn_next_level = Button(cx, cy + 100, 250, 70, "NIVEAU SUIVANT", self.font_medium, bg_color=GREEN)
        self.btn_level_select = Button(cx, cy + 190, 250, 70, "CHOIX NIVEAU", self.font_medium)
        # Boutons du menu pause
        self.btn_resume = Button(cx, cy - 60, 250, 70, "REPRENDRE", self.font_medium, bg_color=GREEN)
        self.btn_pause_quit = Button(cx, cy + 30, 250, 70, "QUITTER", self.font_medium, bg_color=RED)
        # Message temporaire
        self.message = ""
        self.message_color = WHITE
        self.message_timer = 0
        # Popups d'aide et d'infos jeu
        help_content = [
            "=== CONTROLES ===",
            "",
            "Deplacement: Fleches ou WASD",
            "Tirer: ESPACE",
            "Bouclier: B",
            "Mega Tir: MAJ",
            "Plein ecran: F11",
            "Retour menu: ESC",
            "",
            "Detruisez tous les ennemis!",
            "Evitez leurs tirs!"
        ]
        game_content = [
            "=== SI3LN ===",
            "Space Invaders III Last Night",
            "",
            "Un jeu de tir spatial retro",
            "avec 5 mondes differents!",
            "",
            "- Space World",
            "- Desert World",
            "- Forest World",
            "- Marine World",
            "- Apocalyptic World",
            "",
            "Survivez aux vagues d'ennemis",
            "et battez les boss!"
        ]
        self.popup_help = PopUp(400, 500, "AIDE", help_content, self.screen_width, self.screen_height, self.font_small, self.font_large)
        self.popup_game = PopUp(400, 500, "A PROPOS DU JEU", game_content, self.screen_width, self.screen_height, self.font_small, self.font_large)
    
    def update_profile_icon(self):
        """Met à jour l'icône de profil avec le personnage courant"""
        # Si mode invité, récupère le personnage invité
        if self.auth.guest_mode:
            char_idx = self.auth.guest_character
        # Sinon, si utilisateur connecté, récupère le personnage sélectionné
        elif self.auth.current_user:
            char_idx = self.auth.get_user_data("selected_character") or 0
        else:
            char_idx = 0
        # Si l'index est valide, crée l'icône de profil
        if char_idx < len(self.players):
            icon_x = self.screen_width - PROFILE_ICON_SIZE - PROFILE_ICON_POSITION[0]
            icon_y = PROFILE_ICON_POSITION[1]
            self.profile_icon = ProfileIcon(icon_x, icon_y, PROFILE_ICON_SIZE, self.players[char_idx])
        # Met à jour l'index sélectionné
        self.selected_character = char_idx
    
    def show_message(self, text, color=WHITE, duration=MESSAGE_DISPLAY_DURATION):
        """Affiche un message temporaire à l'écran"""
        self.message = text  # Texte du message
        self.message_color = color  # Couleur du message
        self.message_timer = duration  # Durée d'affichage
    
    def handle_events(self):
        """Gère tous les événements du jeu (clavier, souris, fenêtre, etc.)"""
        for event in pygame.event.get():
            # Si l'utilisateur ferme la fenêtre
            if event.type == pygame.QUIT:
                self.running = False
                return
            if event.type == pygame.VIDEORESIZE:
                self.handle_resize(event.w, event.h)
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_F11:
                    self.toggle_fullscreen()
                elif event.key == pygame.K_ESCAPE:
                    if self.profile_screen.active:
                        self.profile_screen.close()
                    elif self.state == STATE_GAMEPLAY:
                        self.state = STATE_LEVEL_SELECT
                    elif self.state == STATE_PAUSE:
                        self.state = STATE_GAMEPLAY
            if self.profile_screen.active:
                if self.profile_screen.handle_event(event):
                    self.update_profile_icon()
                continue
            if self.level_selector.active:
                result = self.level_selector.handle_event(event)
                if result:
                    debug(f"Level selector returned: {result}")
                    if result[0] == "START_LEVEL":
                        self.current_world = result[1]
                        self.current_level = result[2]
                        debug(f"Starting world={self.current_world}, level={self.current_level}")
                        self.level_selector.close()
                        self.start_level()
                    elif result[0] == "BACK":
                        self.level_selector.close()
                        self.state = STATE_MAIN_MENU
            
            # State-specific event handling
            if self.state == STATE_MAIN_MENU:
                self.handle_main_menu_events(event)
            elif self.state == STATE_LOGIN:
                self.handle_login_events(event)
            elif self.state == STATE_REGISTER:
                self.handle_register_events(event)
            elif self.state == STATE_GAMEPLAY:
                self.handle_gameplay_events(event)
            elif self.state == STATE_PAUSE:
                self.handle_pause_events(event)
            elif self.state == STATE_GAME_OVER:
                self.handle_game_over_events(event)
            elif self.state == STATE_LEVEL_WIN:
                self.handle_level_win_events(event)
    
    def handle_main_menu_events(self, event):
        """Gère les événements du menu principal"""
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = event.pos  # Position de la souris
            # Bouton START : mode invité, ouvre le sélecteur de niveau
            if self.btn_start.is_clicked(pos):
                self.auth.login_as_guest(self.selected_character)
                self.update_profile_icon()
                self.level_selector.open()
                self.state = STATE_LEVEL_SELECT
            # Bouton PLAY : va à l'écran de connexion
            elif self.btn_continue.is_clicked(pos):
                self.state = STATE_LOGIN
            # Bouton AIDE : ouvre le popup d'aide
            elif self.btn_help.is_clicked(pos):
                self.popup_help.open()
            # Bouton GAME : ouvre le popup à propos
            elif self.btn_game.is_clicked(pos):
                self.popup_game.open()
            # Bouton QUITTER : ferme le jeu
            elif self.btn_quit.is_clicked(pos):
                self.running = False
            # Clic sur un popup
            if self.popup_help.handle_click(pos) or self.popup_game.handle_click(pos):
                pass
            # Clic sur l'icône de profil
            if self.profile_icon and self.profile_icon.is_clicked(pos):
                self.profile_screen.open()
    
    def handle_login_events(self, event):
        """Gère les événements de l'écran de connexion"""
        self.login_username.handle_event(event)  # Champ pseudo
        self.login_password.handle_event(event)  # Champ mot de passe
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = event.pos
            # Bouton CONNEXION : tente de se connecter
            if self.btn_login.is_clicked(pos):
                username = self.login_username.get_text().strip()
                password = self.login_password.get_text()
                success, msg = self.auth.login(username, password)
                if success:
                    self.show_message(msg, GREEN)
                    self.update_profile_icon()
                    self.level_selector.open()
                    self.state = STATE_LEVEL_SELECT
                    self.login_username.clear()
                    self.login_password.clear()
                else:
                    self.show_message(msg, RED)
            # Bouton Créer un compte : va à l'inscription
            elif self.btn_to_register.is_clicked(pos):
                self.state = STATE_REGISTER
                self.login_username.clear()
                self.login_password.clear()
            # Bouton invité : mode invité
            elif self.btn_guest.is_clicked(pos):
                self.auth.login_as_guest(self.selected_character)
                self.update_profile_icon()
                self.level_selector.open()
                self.state = STATE_LEVEL_SELECT
        # Touche ESC : retour menu principal
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                self.state = STATE_MAIN_MENU
                self.login_username.clear()
                self.login_password.clear()
    
    def handle_register_events(self, event):
        """Gère les événements de l'écran d'inscription"""
        self.register_username.handle_event(event)  # Champ pseudo
        self.register_email.handle_event(event)  # Champ email
        self.register_password.handle_event(event)  # Champ mot de passe
        self.register_confirm.handle_event(event)  # Champ confirmation
        
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = event.pos
            if self.btn_create.is_clicked(pos):
                username = self.register_username.get_text().strip()
                email = self.register_email.get_text().strip()
                password = self.register_password.get_text()
                confirm = self.register_confirm.get_text()

                if password != confirm:
                    self.show_message("Les mots de passe ne correspondent pas", RED)
                    return

                success, msg = self.auth.register(username, password, email)
                if success:
                    self.show_message(msg, GREEN)
                    self.auth.login(username, password)
                    self.update_profile_icon()
                    self.state = STATE_MAIN_MENU
                    self.register_username.clear()
                    self.register_email.clear()
                    self.register_password.clear()
                    self.register_confirm.clear()
                else:
                    self.show_message(msg, RED)

            elif self.btn_back_login.is_clicked(pos):
                self.state = STATE_LOGIN
                self.register_username.clear()
                self.register_email.clear()
                self.register_password.clear()
                self.register_confirm.clear()
        
        # ESC to go back
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                self.state = STATE_LOGIN
    
    def handle_gameplay_events(self, event):
        """Gère les événements pendant le gameplay"""
        if event.type == pygame.KEYDOWN:
            # Touche P : pause
            if event.key == pygame.K_p:
                self.prev_state = self.state
                self.state = STATE_PAUSE
                return
            # Touche ESPACE : tirer
            if event.key == pygame.K_SPACE:
                self.shoot_player_bullet()
            # Touche B : activer le bouclier si bonus actif
            if event.key == pygame.K_b and self.active_bonuses["shield"]["active"]:
                self.activate_shield()
            # Touche SHIFT : mega tir si bonus actif
            if event.key == pygame.K_LSHIFT and self.active_bonuses["mega_shot"]["active"]:
                self.mega_shot()
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = event.pos
            # Clic sur l'icône de profil
            if self.profile_icon and self.profile_icon.is_clicked(pos):
                self.prev_state = self.state
                self.profile_screen.open()
    
    def handle_pause_events(self, event):
        """Gère les événements du menu pause"""
        if event.type == pygame.KEYDOWN:
            # Touche P ou ESC : reprendre le jeu
            if event.key == pygame.K_p or event.key == pygame.K_ESCAPE:
                self.state = STATE_GAMEPLAY
                return
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = event.pos
            # Bouton reprendre : retourne au jeu
            if self.btn_resume.is_clicked(pos):
                self.state = STATE_GAMEPLAY
            # Bouton quitter : retourne à la sélection de niveau
            elif self.btn_pause_quit.is_clicked(pos):
                self.state = STATE_LEVEL_SELECT
    
    def handle_game_over_events(self, event):
        """Gère les événements de l'écran de game over"""
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = event.pos
            # Bouton recommencer : retourne à la sélection de niveau
            if self.btn_restart.is_clicked(pos):
                self.level_selector.open()
                self.state = STATE_LEVEL_SELECT
            # Bouton terminer : retourne au menu principal
            elif self.btn_finish.is_clicked(pos):
                self.state = STATE_MAIN_MENU
            # Clic sur l'icône de profil
            if self.profile_icon and self.profile_icon.is_clicked(pos):
                self.profile_screen.open()
    
    def handle_level_win_events(self, event):
        """Gère les événements de l'écran de victoire de niveau"""
        self.level_selector.handle_event(event)
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = event.pos
            # Sélection d'un niveau suivant
            if self.level_selector.is_level_selected(pos):
                level = self.level_selector.get_selected_level(pos)
                if level:
                    self.selected_level = level
                    self.state = STATE_PLAYER_SELECT
            # Bouton retour : menu principal
            elif self.btn_back.is_clicked(pos):
                self.state = STATE_MAIN_MENU
            # Bouton déconnexion
            elif self.btn_logout.is_clicked(pos):
                self.auth.logout()
                self.update_profile_icon()
                self.state = STATE_MAIN_MENU
        # Touche ESC : retour menu principal
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                self.state = STATE_MAIN_MENU
        # Launch handled in start_level() using the unified `launch_cpp_game` implementation.
    
    def start_level(self):
        """Démarre un nouveau niveau - Lance le moteur C++ si activé, sinon gameplay Python"""
        debug(f"Starting level {self.current_level} in world {self.current_world}")  # Affiche le niveau et le monde
        # Si le moteur C++ est activé, lancer le jeu en C++
        if USE_CPP_ENGINE:
            # Ignore si un lancement est déjà en cours
            if getattr(self, 'launching_cpp', False):
                debug("Launch already in progress; skipping start_level request")
                return
            # Récupérer le mode de fenêtre sélectionné par l'utilisateur
            self.selected_window_mode = self.level_selector.get_selected_mode()
            debug(f"Launching C++ with window mode: {self.selected_window_mode}")
            self.launch_cpp_game()
            return
        # Sinon, continuer avec le gameplay Python
        self.state = STATE_GAMEPLAY  # Passe en mode jeu
        self.lives = MAX_LIVES  # Réinitialise les vies
        # Définit le fond selon le monde
        self.game_bg = self.world_backgrounds.get(self.current_world, self.world_backgrounds["Space"])
        debug(f"Background set for world: {self.current_world}")
        # Crée les bullets avec les couleurs du monde
        if self.current_world in WORLDS and "bullet_colors" in WORLDS[self.current_world]:
            colors = WORLDS[self.current_world]["bullet_colors"]
            self.player_bullet_img = create_bullet_surface(
                colors["player"][0], 
                colors["player"][1], 
                BULLET_SIZE_PLAYER
            )
            self.enemy_bullet_img = create_bullet_surface(
                colors["enemy"][0], 
                colors["enemy"][1], 
                BULLET_SIZE_ENEMY
            )
            debug(f"Bullets created with colors for {self.current_world}")
        # Charge les images d'explosion spécifiques au monde
        explosion_file_map = {
            "Space": ("sprites/player/pb_space.png", "sprites/ennemy/eb_space.png"),
            "Desert": ("sprites/player/pb_desert.png", "sprites/ennemy/eb_desert.png"),
            "Forest": ("sprites/player/pb_forest.png", "sprites/ennemy/eb_forest.png"),
            "Marine": ("sprites/player/pb_marine.png", "sprites/ennemy/eb_marine.png"),
            "Apocalyptic": ("sprites/player/pb_apocaliptyc.png", "sprites/ennemy/eb_apocaliptyc.png")
        }
        if self.current_world in explosion_file_map:
            player_exp_path, enemy_exp_path = explosion_file_map[self.current_world]
            try:
                self.player_explosion_img = safe_load_image(player_exp_path, EXPLOSION_SIZE)
                self.enemy_explosion_img = safe_load_image(enemy_exp_path, EXPLOSION_SIZE)
                debug(f"Loaded explosion images for {self.current_world}")
            except Exception as e:
                warning(f"Could not load explosion images: {e}")
                self.player_explosion_img = None
                self.enemy_explosion_img = None
        else:
            self.player_explosion_img = None
            self.enemy_explosion_img = None
        # Vide toutes les entités
        self.enemies.empty()
        self.player_bullets.empty()
        self.enemy_bullets.empty()
        self.explosions.empty()
        self.bonuses.empty()
        self.special_attacks.empty()
        # Réinitialise les malus
        self.player_debuffs = {
            "frozen": False,
            "blinded": False,
            "rooted": False,
            "timer": 0,
            "duration": 0
        }
        # Crée le joueur
        player_img = self.players_gameplay[self.selected_character]
        self.player = Player(self.screen_width // 2, 
                            self.screen_height - PLAYER_START_Y_OFFSET,
                            player_img,
                            self.screen_width,
                            self.screen_height)
        # Crée les ennemis
        debug("Spawning enemies...")
        self.spawn_enemies()
        debug(f"Level started successfully! Enemies: {len(self.enemies)}")
        debug(f"Player created: {self.player is not None}")
    def spawn_enemies(self):
        """Fait apparaître les ennemis pour le niveau courant"""
        self.entity_manager.spawn_enemies()  # Délègue à l'EntityManager
    
    def shoot_player_bullet(self):
        """Le joueur tire une balle"""
        self.entity_manager.shoot_player_bullet()  # Délègue à l'EntityManager
    
    def activate_shield(self):
        """Active le bouclier du joueur"""
        if self.active_bonuses["shield"]["active"]:
            self.active_bonuses["shield"]["active"] = False  # Désactive le bonus
            self.show_message("Bouclier activé!", BLUE)
            # Ici tu peux ajouter un effet visuel de bouclier
    
    def mega_shot(self):
        """Tir spécial plus puissant"""
        if self.active_bonuses["mega_shot"]["active"]:
            self.entity_manager.shoot_mega_shot()  # Délègue à l'EntityManager
            self.active_bonuses["mega_shot"]["active"] = False  # Désactive le bonus
            self.show_message("Mega tir activé!", YELLOW)
    
    def spawn_bonus(self, x, y):
        """Fait tomber un bonus aléatoire"""
        self.entity_manager.spawn_bonus(x, y)  # Délègue à l'EntityManager
    
    def activate_bonus(self, bonus_type):
        """Active un bonus"""
        if bonus_type == "life":
            self.lives = min(self.lives + 1, 10)  # Ajoute une vie (max 10)
            self.show_message("+1 Vie!", GREEN)
        else:
            self.active_bonuses[bonus_type]["active"] = True  # Active le bonus
            self.active_bonuses[bonus_type]["timer"] = pygame.time.get_ticks()  # Démarre le timer
            self.show_message(f"{bonus_type.title()} activé!", BLUE if bonus_type == "shield" else YELLOW)
    
    def trigger_world_special(self):
        """Déclenche une attaque spéciale selon le monde"""
        world = self.current_world
        # Selon le monde, déclenche l'effet spécial
        if world == "Space":
            self.spawn_space_lasers()
        elif world == "Desert":
            self.blind_player()
        elif world == "Forest": 
            self.root_player()
        elif world == "Marine":
            self.spawn_ice_ball()
        elif world == "Apocalyptic":
            self.spawn_energy_ball()
    
    def spawn_space_lasers(self):
        """1 à 3 rayons laser tombent au hasard"""
        num_lasers = random.randint(1, min(3, self.current_level))  # Nombre de lasers
        for _ in range(num_lasers):
            attack = SpecialAttack("laser", "Space", self.current_level, 
                                 self.screen_width, self.screen_height)
            self.special_attacks.add(attack)  # Ajoute l'attaque spéciale
        self.show_message("Rayons laser!", PURPLE)
    
    def spawn_ice_ball(self):
        """Fait tomber une boule de glace qui peut geler le joueur"""
        attack = SpecialAttack("ice", "Marine", self.current_level,
                              self.screen_width, self.screen_height)  # Crée l'attaque spéciale
        self.special_attacks.add(attack)  # Ajoute à la liste des attaques spéciales
        self.show_message("Balle de glace!", LIGHT_BLUE)
    
    def spawn_energy_ball(self):
        """Fait tomber une boule d'énergie qui explose"""
        attack = SpecialAttack("energy", "Apocalyptic", self.current_level,
                              self.screen_width, self.screen_height)  # Crée l'attaque spéciale
        self.special_attacks.add(attack)  # Ajoute à la liste des attaques spéciales
        self.show_message("Boule d'énergie!", YELLOW)
    
    def blind_player(self):
        """Rend le joueur aveugle temporairement (nuage de sable)"""
        attack = SpecialAttack("sand", "Desert", self.current_level,
                              self.screen_width, self.screen_height)  # Crée l'attaque spéciale
        self.special_attacks.add(attack)  # Ajoute à la liste des attaques spéciales
        self.player_debuffs["blinded"] = True  # Active le malus
        self.player_debuffs["timer"] = pygame.time.get_ticks()  # Démarre le timer
        self.player_debuffs["duration"] = min(1000 + (self.current_level * 200), 3000)  # Durée max 3s
        self.show_message("Nuage de sable!", SAND_COLOR)
    
    def root_player(self):
        """Immobilise le joueur temporairement (racines)"""
        attack = SpecialAttack("roots", "Forest", self.current_level,
                              self.screen_width, self.screen_height)  # Crée l'attaque spéciale
        self.special_attacks.add(attack)  # Ajoute à la liste des attaques spéciales
        self.player_debuffs["rooted"] = True  # Active le malus
        self.player_debuffs["timer"] = pygame.time.get_ticks()  # Démarre le timer
        self.player_debuffs["duration"] = min(
            DEBUFF_ROOTED_BASE_DURATION + (self.current_level * DEBUFF_ROOTED_LEVEL_MULTIPLIER),
            DEBUFF_DURATION_MAX
        )  # Durée calculée
        self.show_message("Racines!", BROWN)
    
    def load_player_animations(self, player_index):
        """Charge paresseusement les animations du joueur (pour AnimatedPlayer dans le profil)"""
        if player_index >= len(self.players_animation_folders) or self.players_animation_folders[player_index] is None:
            return None  # Aucun dossier d'animation
        folder = self.players_animation_folders[player_index]
        # Charge toutes les frames d'animation du dossier
        frames = []
        try:
            png_files = sorted([f for f in os.listdir(folder) if f.lower().endswith(".png")])
            info(f"Loading animations for player_{player_index + 1} ({len(png_files)} frames)...")
            start_time = pygame.time.get_ticks()
            for png_file in png_files:
                try:
                    frame_path = os.path.join(folder, png_file)
                    frame = pygame.image.load(frame_path)
                    frames.append(frame)
                except Exception as e:
                    warning(f"Could not load frame {png_file}: {e}")
            elapsed = pygame.time.get_ticks() - start_time
            info(f"Loaded {len(frames)} animation frames in {elapsed}ms")
        except Exception as e:
            error(f"Error loading animations: {e}")
        return frames if frames else None
    
    def update(self):
        """Met à jour la logique du jeu"""
        # Met à jour le timer du message
        if self.message_timer > 0:
            self.message_timer -= 1
            if self.message_timer == 0:
                self.message = ""
        # Met à jour l'écran de profil
        if self.profile_screen.active:
            self.profile_screen.update()
            new_char = self.profile_screen.get_selected_character()
            if new_char != self.selected_character:
                self.selected_character = new_char
                self.update_profile_icon()
            return
        # Met à jour le sélecteur de niveau
        if self.level_selector.active:
            self.level_selector.update()
            return
        # Met à jour les boutons de l'UI
        mouse_pos = pygame.mouse.get_pos()
        if self.state == STATE_MAIN_MENU:
            self.btn_start.update(mouse_pos)
            self.btn_continue.update(mouse_pos)
            self.btn_help.update(mouse_pos)
            self.btn_game.update(mouse_pos)
            self.btn_quit.update(mouse_pos)
            self.popup_help.update(mouse_pos)
            self.popup_game.update(mouse_pos)
        elif self.state == STATE_LOGIN:
            self.login_username.update()
            self.login_password.update()
            self.btn_login.update(mouse_pos)
            self.btn_to_register.update(mouse_pos)
            self.btn_guest.update(mouse_pos)
        elif self.state == STATE_REGISTER:
            self.register_username.update()
            self.register_email.update()
            self.register_password.update()
            self.register_confirm.update()
            self.btn_register.update(mouse_pos)
            self.btn_back_login.update(mouse_pos)
        elif self.state == STATE_GAMEPLAY:
            self.update_gameplay()
        elif self.state == STATE_PAUSE:
            # Met à jour les boutons du menu pause
            mouse_pos = pygame.mouse.get_pos()
            self.btn_resume.update(mouse_pos)
            self.btn_pause_quit.update(mouse_pos)
        elif self.state == STATE_GAME_OVER:
            self.btn_restart.update(mouse_pos)
            self.btn_finish.update(mouse_pos)
        elif self.state == STATE_LEVEL_WIN:
            self.btn_next_level.update(mouse_pos)
            self.btn_level_select.update(mouse_pos)
        # Met à jour l'icône de profil
        if self.profile_icon:
            self.profile_icon.update(mouse_pos)
    
    def update_gameplay(self):
        """Update gameplay logic"""
        if not self.player:
            debug("No player!")
            return
        
        debug(f"In update_gameplay - Enemies: {len(self.enemies)}, State: {self.state}")
        
        # Update player (sauf si rooté)
        keys = pygame.key.get_pressed()
        if not self.player_debuffs["rooted"]:
            self.player.update(keys)
        
        # Update bullets
        self.player_bullets.update()
        self.enemy_bullets.update()
        
        # Update enemies
        for enemy in self.enemies:
            enemy.update()
            
            # Enemy shooting
            current_time = pygame.time.get_ticks()
            if enemy.can_shoot() and random.random() < enemy.shoot_chance:
                bullet = Bullet(enemy.rect.centerx,
                            enemy.rect.bottom,
                            self.enemy_bullet_img,
                            False,
                            self.screen_height)
                self.enemy_bullets.add(bullet)
                enemy.last_shot = current_time
        
        # Update explosions
        self.explosions.update()
        
        # Update bonuses
        self.bonuses.update()
        
        # Update special attacks
        self.special_attacks.update()
        
        # Gestion des debuffs
        self.update_debuffs()
        
        # Chance de spawner un bonus
        if random.random() < 0.001:  # 0.1% de chance par frame
            x = random.randint(50, self.screen_width - 50)
            self.spawn_bonus(x, 0)
        
        # Chance de déclencher une attaque spéciale
        current_time = pygame.time.get_ticks()
        if (current_time - self.last_special_attack_time > self.special_attack_cooldown and 
            random.random() < 0.01):  # 1% de chance quand le cooldown est écoulé
            self.trigger_world_special()
            self.last_special_attack_time = current_time
        
        # Clean up dead sprites to prevent memory leaks
        self.cleanup_sprites()
        
        # Collision detection
        self.check_collisions()
        
        debug(f"Before win check - Enemies: {len(self.enemies)}")
        
        # Check win condition
        if len(self.enemies) == 0:
            debug("WIN CONDITION TRIGGERED!")
            self.state = STATE_LEVEL_WIN
            if self.auth.current_user:
                self.auth.update_user_data(
                    high_score=max(self.current_score, 
                                  self.auth.get_user_data("high_score") or 0)
                )
    
    def update_debuffs(self):
        """Gère la durée des debuffs"""
        current_time = pygame.time.get_ticks()
        # Fin du malus "aveugle"
        if self.player_debuffs["blinded"]:
            if current_time - self.player_debuffs["timer"] > self.player_debuffs["duration"]:
                self.player_debuffs["blinded"] = False
        # Fin du malus "immobilisé"
        if self.player_debuffs["rooted"]:
            if current_time - self.player_debuffs["timer"] > self.player_debuffs["duration"]:
                self.player_debuffs["rooted"] = False
    
    def check_collisions(self):
        """Vérifie toutes les collisions"""
        self.collision_manager.check_all_collisions()  # Délègue à CollisionManager
    
    def draw(self):
        """Dessine tous les éléments à l'écran"""
        # Dessine selon l'état du jeu
        if self.level_selector.active:
            self.level_selector.draw(self.menu_bg)
        elif self.state == STATE_MAIN_MENU:
            self.draw_main_menu()
        elif self.state == STATE_LOGIN:
            self.draw_login()
        elif self.state == STATE_REGISTER:
            self.draw_register()
        elif self.state == STATE_GAMEPLAY:
            self.draw_gameplay()
        elif self.state == STATE_PAUSE:
            # Affiche le gameplay en fond (figé)
            self.draw_gameplay()
            # Affiche le menu pause par-dessus
            self.draw_pause_menu()
        elif self.state == STATE_GAME_OVER:
            self.draw_game_over()
        elif self.state == STATE_LEVEL_WIN:
            self.draw_level_win()
        # Dessine l'icône de profil
        if (not self.level_selector.active and 
            self.state not in [STATE_LOGIN, STATE_REGISTER]):
            if self.profile_icon:
                self.profile_icon.draw(self.screen)
        
        # Draw profile screen
        if self.profile_screen.active:
            self.profile_screen.draw()

        # Draw launch popup if present
        if hasattr(self, 'launch_popup') and self.launch_popup and self.launch_popup.visible:
            self.launch_popup.draw(self.screen)
            self.launch_popup.update(pygame.mouse.get_pos())
        
        # Draw message
        if self.message:
            msg_surf = self.font_small.render(self.message, True, self.message_color)
            msg_rect = msg_surf.get_rect(center=(self.screen_width // 2, 50))
            bg_rect = msg_rect.inflate(20, 10)
            pygame.draw.rect(self.screen, (0, 0, 0, 200), bg_rect, border_radius=5)
            self.screen.blit(msg_surf, msg_rect)
        
        pygame.display.flip()
    
    def draw_main_menu(self):
        """Dessine le menu principal"""
        self.screen.blit(self.menu_bg, (0, 0))  # Fond du menu
        # Titre principal
        title = self.font_large.render("S I 3 L N", True, WHITE)
        title_rect = title.get_rect(center=(self.screen_width // 2, 150))
        shadow = self.font_large.render("S I 3 L N", True, BLACK)
        shadow_rect = title_rect.copy()
        shadow_rect.x += 3
        shadow_rect.y += 3
        self.screen.blit(shadow, shadow_rect)
        self.screen.blit(title, title_rect)
        # Sous-titre
        subtitle = self.font_small.render("Space Invaders III - Last Night", True, CYAN)
        subtitle_rect = subtitle.get_rect(center=(self.screen_width // 2, 220))
        self.screen.blit(subtitle, subtitle_rect)
        # Boutons principaux
        self.btn_start.draw(self.screen)
        self.btn_continue.draw(self.screen)
        self.btn_help.draw(self.screen)
        self.btn_game.draw(self.screen)
        self.btn_quit.draw(self.screen)
        # Popups d'aide et à propos
        self.popup_help.draw(self.screen)
        self.popup_game.draw(self.screen)
    
    def draw_login(self):
        """Dessine l'écran de connexion"""
        self.screen.blit(self.menu_bg, (0, 0))  # Fond du menu
        # Overlay sombre
        overlay = pygame.Surface((self.screen_width, self.screen_height), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 150))
        self.screen.blit(overlay, (0, 0))
        # Titre
        title = self.font_large.render("CONNEXION", True, WHITE)
        title_rect = title.get_rect(center=(self.screen_width // 2, 150))
        self.screen.blit(title, title_rect)
        # Champs de saisie
        self.login_username.draw(self.screen)
        self.login_password.draw(self.screen)
        # Boutons
        self.btn_login.draw(self.screen)
        self.btn_to_register.draw(self.screen)
        self.btn_guest.draw(self.screen)
    
    def draw_register(self):
        """Dessine l'écran d'inscription"""
        self.screen.blit(self.menu_bg, (0, 0))  # Fond du menu
        # Overlay sombre
        overlay = pygame.Surface((self.screen_width, self.screen_height), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 150))
        self.screen.blit(overlay, (0, 0))
        # Titre
        title = self.font_large.render("INSCRIPTION", True, WHITE)
        title_rect = title.get_rect(center=(self.screen_width // 2, 120))
        self.screen.blit(title, title_rect)
        # Champs de saisie
        self.register_username.draw(self.screen)
        self.register_email.draw(self.screen)
        self.register_password.draw(self.screen)
        self.register_confirm.draw(self.screen)
        # Boutons
        self.btn_register.draw(self.screen)
        self.btn_back_login.draw(self.screen)
    
    def draw_gameplay(self):
        """Dessine l'écran de jeu (gameplay)"""
        self.screen.blit(self.game_bg, (0, 0))  # Fond du jeu
        # Entités
        if self.player:
            self.screen.blit(self.player.image, self.player.rect)
        self.enemies.draw(self.screen)
        self.player_bullets.draw(self.screen)
        self.enemy_bullets.draw(self.screen)
        self.explosions.draw(self.screen)
        self.bonuses.draw(self.screen)
        self.special_attacks.draw(self.screen)
        # Effet de malus "aveugle"
        if self.player_debuffs["blinded"]:
            overlay = pygame.Surface((self.screen_width, self.screen_height), pygame.SRCALPHA)
            overlay.fill((210, 180, 140, 150))
            self.screen.blit(overlay, (0, 0))
        # HUD
        self.draw_hud()
    
    def draw_hud(self):
        """Dessine le HUD (barre d'information en haut)"""
        hud_panel = pygame.Surface((self.screen_width, 50), pygame.SRCALPHA)
        hud_panel.fill((0, 0, 0, 180))
        self.screen.blit(hud_panel, (0, 0))
        
        score_text = self.font_small.render(f"Score: {self.current_score}", True, WHITE)
        self.screen.blit(score_text, (20, 15))
        
        level_text = self.font_small.render(f"Niveau: {self.current_level}", True, CYAN)
        level_rect = level_text.get_rect(center=(self.screen_width // 2, 25))
        self.screen.blit(level_text, level_rect)
        
        lives_text = self.font_small.render(f"Vies: {self.lives}", True, RED)
        lives_rect = lives_text.get_rect(right=self.screen_width - 120, centery=25)
        self.screen.blit(lives_text, lives_rect)
        
        # Afficher les bonus actifs
        bonus_x = self.screen_width - 250
        if self.active_bonuses["shield"]["active"]:
            shield_text = self.font_tiny.render("BOUCLIER", True, BLUE)
            self.screen.blit(shield_text, (bonus_x, 15))
            bonus_x += 80
        
        if self.active_bonuses["mega_shot"]["active"]:
            mega_text = self.font_tiny.render("MEGA TIR", True, YELLOW)
            self.screen.blit(mega_text, (bonus_x, 15))
    
    def draw_game_over(self):
        """Dessine l'écran de game over"""
        self.screen.blit(self.menu_bg, (0, 0))  # Fond du menu
        # Overlay sombre
        overlay = pygame.Surface((self.screen_width, self.screen_height), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 180))
        self.screen.blit(overlay, (0, 0))
        # Titre
        title = self.font_large.render("GAME OVER", True, RED)
        title_rect = title.get_rect(center=(self.screen_width // 2, 120))
        self.screen.blit(title, title_rect)
        # Score final
        score_text = self.font_medium.render(f"Score Final: {self.current_score}", True, WHITE)
        score_rect = score_text.get_rect(center=(self.screen_width // 2, 200))
        self.screen.blit(score_text, score_rect)
        # Niveau atteint
        level_text = self.font_small.render(f"Niveau atteint: {self.current_level}", True, CYAN)
        level_rect = level_text.get_rect(center=(self.screen_width // 2, 250))
        self.screen.blit(level_text, level_rect)
        # Tableau des meilleurs scores
        self.draw_high_scores(300)
        # Boutons
        self.btn_restart.draw(self.screen)
        self.btn_finish.draw(self.screen)
    
    def draw_high_scores(self, start_y):
        """Dessine le tableau des meilleurs scores"""
        title = self.font_medium.render("MEILLEURS SCORES", True, YELLOW)
        title_rect = title.get_rect(center=(self.screen_width // 2, start_y))
        self.screen.blit(title, title_rect)
        scores = self.score_manager.get_top_scores(10)
        y = start_y + 50
        for i, entry in enumerate(scores):
            rank_color = YELLOW if i < 3 else WHITE
            text = f"{i+1}. {entry['username'][:15]:15s} - {entry['score']:6d} pts (Niv {entry['level']})"
            score_surf = self.font_tiny.render(text, True, rank_color)
            score_rect = score_surf.get_rect(center=(self.screen_width // 2, y))
            self.screen.blit(score_surf, score_rect)
            y += 25
            if y > self.screen_height - 150:
                break
    
    def draw_pause_menu(self):
        """Dessine le menu pause (overlay)"""
        # Overlay semi-transparent
        overlay = pygame.Surface((self.screen_width, self.screen_height), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 180))  # Sombre avec transparence
        self.screen.blit(overlay, (0, 0))
        # Titre pause
        pause_text = self.font_large.render("PAUSE", True, WHITE)
        pause_rect = pause_text.get_rect(center=(self.screen_width // 2, self.screen_height // 2 - 200))
        self.screen.blit(pause_text, pause_rect)
        # Centre les boutons
        cx = self.screen_width // 2
        cy = self.screen_height // 2
        self.btn_resume.rect.centerx = cx
        self.btn_resume.rect.centery = cy - 40
        self.btn_pause_quit.rect.centerx = cx
        self.btn_pause_quit.rect.centery = cy + 50
        # Dessine les boutons
        self.btn_resume.draw(self.screen)
        self.btn_pause_quit.draw(self.screen)
        # Astuce
        hint_text = self.font_small.render("Appuyez sur P ou ESC pour reprendre", True, GRAY)
        hint_rect = hint_text.get_rect(center=(self.screen_width // 2, self.screen_height // 2 + 150))
        self.screen.blit(hint_text, hint_rect)
    
    def draw_level_win(self):
        """Dessine l'écran de victoire de niveau"""
        self.screen.blit(self.menu_bg, (0, 0))  # Fond du menu
        # Overlay sombre
        overlay = pygame.Surface((self.screen_width, self.screen_height), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 150))
        self.screen.blit(overlay, (0, 0))
        
        title = self.font_large.render(f"NIVEAU {self.current_level} TERMINÉ!", True, GREEN)
        title_rect = title.get_rect(center=(self.screen_width // 2, 200))
        self.screen.blit(title, title_rect)
        
        score_text = self.font_medium.render(f"Score: {self.current_score}", True, WHITE)
        score_rect = score_text.get_rect(center=(self.screen_width // 2, 280))
        self.screen.blit(score_text, score_rect)
        
        self.btn_next_level.draw(self.screen)
        self.btn_level_select.draw(self.screen)
    
    def toggle_fullscreen(self):
        """Active/désactive le mode plein écran avec gestion d'erreur"""
        try:
            # Débounce pour éviter les toggles rapides successifs (ex: key repeat)
            now = pygame.time.get_ticks()
            if now - getattr(self, 'last_fullscreen_toggle_time', 0) < 300:
                debug("Ignored rapid fullscreen toggle")
                return
            self.last_fullscreen_toggle_time = now

            self.is_fullscreen = not getattr(self, 'is_fullscreen', False)  # Inverse l'état
            if self.is_fullscreen:
                self.screen = pygame.display.set_mode(
                    (0, 0), pygame.FULLSCREEN | pygame.RESIZABLE
                )
            else:
                self.screen = pygame.display.set_mode(
                    (DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT),
                    pygame.RESIZABLE
                )
            self.screen_width = self.screen.get_width()
            self.screen_height = self.screen.get_height()
            # Recharge les assets et l'UI
            self.load_assets()
            self.create_ui()
            self.profile_screen = ProfileScreen(self.screen, self.auth, self.players)
            self.level_selector = LevelSelector(self.screen, WORLDS)
            self.update_profile_icon()
            info(f"Fullscreen toggled: {self.is_fullscreen}")
        except Exception as e:
            error(f"Error toggling fullscreen: {e}")
            self.is_fullscreen = not self.is_fullscreen  # Annule en cas d'erreur
    
    def handle_resize(self, width, height):
        """Gère le redimensionnement de la fenêtre et recrée l'UI"""
        # Ignore les évènements redondants si la taille n'a pas changé
        if width == getattr(self, 'screen_width', None) and height == getattr(self, 'screen_height', None):
            return
        # Vérifie la taille minimale
        if width < 800 or height < 600:
            warning(f"Window too small ({width}x{height}), minimum is 800x600")
            return
        self.screen_width = width
        self.screen_height = height
        self.screen = pygame.display.set_mode((width, height), pygame.RESIZABLE)
        try:
            # Recharge les fonds avec la nouvelle taille
            self.menu_bg = load_image("worlds/home_page.jpg", 
                                      (self.screen_width, self.screen_height), False)
            self.world_backgrounds = {}
            for world_key, world_data in WORLDS.items():
                try:
                    bg_path = f"worlds/{world_data['background']}"
                    self.world_backgrounds[world_key] = load_image(bg_path, 
                                                                  (self.screen_width, self.screen_height), False)
                except:
                    pass  # Garde l'ancien fond si erreur
            self.game_bg = self.world_backgrounds.get("Space", self.game_bg)
            # Recrée l'UI avec les nouvelles dimensions
            self.create_ui()
            self.profile_screen = ProfileScreen(self.screen, self.auth, self.players)
            self.level_selector = LevelSelector(self.screen, WORLDS)
            self.update_profile_icon()
            info(f"Window resized to {width}x{height}")
        except Exception as e:
            error(f"Error during resize: {e}")
    
    def cleanup_sprites(self):
        """Nettoie et supprime tous les sprites morts"""
        self.entity_manager.cleanup_sprites()  # Délègue à l'EntityManager
    
    def run(self):
        """Boucle principale du jeu"""
        while self.running:
            self.handle_events()  # Gère les événements
            self.update()         # Met à jour la logique
            self.draw()           # Dessine l'écran
            self.clock.tick(FPS) # Limite le framerate
        pygame.quit()
        sys.exit()


if __name__ == "__main__":
    game = Game()
    game.run()

class AnimatedPlayer:
    """Affiche un personnage animé à partir de frames avec support du lazy loading"""
    def __init__(self, x, y, width, height, player_index, animation_folder=None, game=None):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.player_index = player_index
        self.animation_folder = animation_folder
        self.game = game

        self.frames = []
        self.load_frames()

        self.current_frame = 0
        self.frame_delay = 1/24  # 24 fps
        self.elapsed_time = 0.0
        self.is_animating = True
        self.loop = True
        self.rect = pygame.Rect(x, y, width, height)

    def load_frames(self):
        """Load all animation frames for the player (with lazy loading support)"""
        import re  # <-- FIX: import re at the top of the function, not inside a nested function

        if self.animation_folder:
            player_path = self.animation_folder
        else:
            player_path = f"assets/players/player_{self.player_index + 1}"

        if not os.path.exists(player_path):
            warning(f"Dossier introuvable : {player_path}")
            return

        frame_files = sorted([
            f for f in os.listdir(player_path)
            if (f.lower().startswith('frame_') or f.lower().startswith('animatediff_'))
            and f.lower().endswith('.png')
        ])

        def get_frame_number(filename):
            if filename.lower().startswith('frame_'):
                try:
                    return int(filename.split('_')[1].split('.')[0])
                except Exception:
                    return 999999
            else:  # AnimateDiff format
                match = re.search(r'\.(\d+)\.png', filename)
                if match:
                    return int(match.group(1))
                return 999999

        frame_files.sort(key=get_frame_number)

        info(f"Loading {len(frame_files)} animation frames for player_{self.player_index + 1}...")
        start_time = pygame.time.get_ticks()

        for frame_file in frame_files:
            frame_path = os.path.join(player_path, frame_file)
            try:
                image = pygame.image.load(frame_path)
                scaled = pygame.transform.scale(image, (self.width, self.height))
                self.frames.append(scaled)
            except pygame.error as e:
                warning(f"Impossible de charger {frame_path}: {e}")

        elapsed = pygame.time.get_ticks() - start_time
        if self.frames:
            info(f"{len(self.frames)} frames chargées pour player_{self.player_index + 1} en {elapsed}ms")
        else:
            warning(f"Aucune frame chargée pour player_{self.player_index + 1}")

    def update(self, dt=1/60):
        """Update animation frame"""
        if not self.frames or not self.is_animating:
            return

        self.elapsed_time += dt

        if self.elapsed_time >= self.frame_delay:
            self.elapsed_time -= self.frame_delay
            self.current_frame += 1

            if self.current_frame >= len(self.frames):
                if self.loop:
                    self.current_frame = 0
                else:
                    self.current_frame = len(self.frames) - 1
                    self.is_animating = False

    def draw(self, screen):
        """Draw current frame"""
        if not self.frames:
            return

        current_frame_index = min(self.current_frame, len(self.frames) - 1)
        frame = self.frames[current_frame_index]
        screen.blit(frame, (self.x, self.y))

    def reset(self):
        """Reset animation to first frame"""
        self.current_frame = 0
        self.elapsed_time = 0.0
        self.is_animating = True
