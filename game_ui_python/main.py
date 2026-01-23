import pygame
import sys
import subprocess
import os

pygame.init()
pygame.font.init()

WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("SI3LN - Menu Principal")

font = pygame.font.SysFont(None, 48)
menu_items = ["Jouer", "Backgrounds", "Niveaux", "Personnages", "Quitter"]
selected = 0

ASSET_DIR = "/home/ramos/SI3LN/game_engine_C++/build/assets"
HOME_BG_PATH = os.path.join(ASSET_DIR, "worlds/home_page.jpg")

def load_home_background():
    if os.path.exists(HOME_BG_PATH):
        try:
            img = pygame.image.load(HOME_BG_PATH)
            return pygame.transform.scale(img, (WIDTH, HEIGHT))
        except Exception as e:
            print(f"Erreur chargement home_page.jpg: {e}")
    # Background par défaut
    surf = pygame.Surface((WIDTH, HEIGHT))
    surf.fill((30, 30, 30))
    return surf

# Liste des backgrounds pour le sous-menu
def get_backgrounds():
    bg_dir = os.path.join(ASSET_DIR, "worlds")
    if os.path.exists(bg_dir):
        bg_files = [f for f in os.listdir(bg_dir) if f.startswith("background_")]
        return bg_files
    return []

background_files = get_backgrounds()
background_selected = 0

def load_background(idx):
    if idx < len(background_files):
        bg_path = os.path.join(ASSET_DIR, "worlds", background_files[idx])
        if os.path.exists(bg_path):
            try:
                img = pygame.image.load(bg_path)
                return pygame.transform.scale(img, (WIDTH, HEIGHT))
            except Exception as e:
                print(f"Erreur chargement background: {e}")
    # Background par défaut
    surf = pygame.Surface((WIDTH, HEIGHT))
    surf.fill((30, 30, 30))
    return surf

background_img = load_home_background()

# Chemin vers un asset exemple (image ennemi)
ENEMY_PATH = os.path.join(ASSET_DIR, "enemies/Apocalyptic_world/enemy (1).png")
enemy_img = None
if os.path.exists(ENEMY_PATH):
    try:
        enemy_img = pygame.image.load(ENEMY_PATH)
        enemy_img = pygame.transform.scale(enemy_img, (100, 100))
    except Exception as e:
        print(f"Erreur chargement image ennemi: {e}")
else:
    print(f"Image ennemi non trouvée: {ENEMY_PATH}")

def draw_menu():
    # Affiche le background
    screen.blit(background_img, (0, 0))
    # Affiche l'image de l'ennemi en haut à droite
    if enemy_img:
        screen.blit(enemy_img, (WIDTH - 120, 20))
    else:
        # Image par défaut si asset manquant
        pygame.draw.rect(screen, (255, 0, 0), (WIDTH - 120, 20, 100, 100))
    # Affiche les items du menu
    for i, item in enumerate(menu_items):
        color = (255, 255, 0) if i == selected else (200, 200, 200)
        text = font.render(item, True, color)
        rect = text.get_rect(center=(WIDTH//2, 180 + i*60))
        screen.blit(text, rect)
    # Si le sous-menu backgrounds est sélectionné, affiche la liste si elle existe
    if menu_items[selected] == "Backgrounds":
        sub_font = pygame.font.SysFont(None, 32)
        if background_files:
            for j, bg in enumerate(background_files):
                color = (0, 255, 255) if j == background_selected else (180, 180, 180)
                txt = sub_font.render(bg, True, color)
                rct = txt.get_rect(center=(WIDTH//2, 500 + j*35))
                screen.blit(txt, rct)
        else:
            txt = sub_font.render("Aucun background disponible", True, (255, 0, 0))
            rct = txt.get_rect(center=(WIDTH//2, 500))
            screen.blit(txt, rct)
    pygame.display.flip()

def launch_cpp_game():
    # Affiche un écran d'attente
    screen.fill((0, 0, 0))
    wait_font = pygame.font.SysFont(None, 40)
    wait_text = wait_font.render("Chargement du jeu...", True, (255, 255, 255))
    wait_rect = wait_text.get_rect(center=(WIDTH//2, HEIGHT//2))
    screen.blit(wait_text, wait_rect)
    pygame.display.flip()
    pygame.time.wait(1000)  # 1 seconde d'attente
    pygame.quit()
    
    # IMPORTANT: Lance le moteur C++ DANS SON DOSSIER pour que les assets soient trouvés
    game_dir = "/home/ramos/SI3LN/game_engine_C++/build"
    game_exe = os.path.join(game_dir, "SI3LN")
    subprocess.run([game_exe], cwd=game_dir)
    sys.exit()

while True:
    draw_menu()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if menu_items[selected] == "Backgrounds":
                if background_files:
                    if event.key == pygame.K_UP:
                        background_selected = (background_selected - 1) % len(background_files)
                        background_img = load_background(background_selected)
                    elif event.key == pygame.K_DOWN:
                        background_selected = (background_selected + 1) % len(background_files)
                        background_img = load_background(background_selected)
                if event.key == pygame.K_LEFT:
                    selected = (selected - 1) % len(menu_items)
                elif event.key == pygame.K_RIGHT:
                    selected = (selected + 1) % len(menu_items)
            else:
                if event.key == pygame.K_UP:
                    selected = (selected - 1) % len(menu_items)
                elif event.key == pygame.K_DOWN:
                    selected = (selected + 1) % len(menu_items)
                elif event.key == pygame.K_RETURN:
                    if menu_items[selected] == "Jouer":
                        launch_cpp_game()
                    elif menu_items[selected] == "Quitter":
                        pygame.quit()
                        sys.exit()
                    # Ajoute ici la logique pour "Niveaux" et "Personnages"
    pygame.time.Clock().tick(30)
