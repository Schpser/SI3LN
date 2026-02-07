"""
Level selection screen for SI3LN Game
Allows selection of worlds and levels within worlds
"""
import pygame
from constants import *
from ui_components import Button, Panel
from utils import load_image


class WorldCard:
    """Represents a world selection card with background image"""
    def __init__(self, x, y, width, height, world_name, world_data, bg_image):
        self.rect = pygame.Rect(x, y, width, height)
        self.world_name = world_name
        self.world_data = world_data
        self.bg_image = bg_image
        self.hovered = False
        self.selected = False
        
        # Scale background image to card size
        if self.bg_image:
            self.bg_image = pygame.transform.scale(self.bg_image, (width, height))
        
        # Font for world name
        self.font = pygame.font.Font(None, 40)
        self.font_small = pygame.font.Font(None, 24)
    
    def draw(self, screen):
        """Draw the world card"""
        # Draw background image with rounded corners
        if self.bg_image:
            # Create a surface with rounded rectangle mask
            rounded_surface = pygame.Surface((self.rect.width, self.rect.height), pygame.SRCALPHA)
            # Draw rounded rectangle as mask
            pygame.draw.rect(rounded_surface, (255, 255, 255, 255), (0, 0, self.rect.width, self.rect.height), border_radius=10)
            # Create temp surface for the image
            temp_surface = pygame.Surface((self.rect.width, self.rect.height), pygame.SRCALPHA)
            temp_surface.blit(self.bg_image, (0, 0))
            # Apply the mask by using per-pixel alpha
            temp_surface.blit(rounded_surface, (0, 0), special_flags=pygame.BLEND_RGBA_MIN)
            # Draw the masked image
            screen.blit(temp_surface, self.rect)
        else:
            pygame.draw.rect(screen, (30, 30, 60), self.rect, border_radius=10)
        
        # Draw overlay for visibility
        overlay = pygame.Surface((self.rect.width, self.rect.height), pygame.SRCALPHA)
        # Create rounded overlay
        pygame.draw.rect(overlay, (0, 0, 0, 0), (0, 0, self.rect.width, self.rect.height), border_radius=10)
        if self.selected:
            pygame.draw.rect(overlay, (0, 200, 255, 80), (0, 0, self.rect.width, self.rect.height), border_radius=10)
        elif self.hovered:
            pygame.draw.rect(overlay, (255, 255, 255, 40), (0, 0, self.rect.width, self.rect.height), border_radius=10)
        else:
            pygame.draw.rect(overlay, (0, 0, 0, 60), (0, 0, self.rect.width, self.rect.height), border_radius=10)
        screen.blit(overlay, self.rect)
        
        # Draw border
        border_color = CYAN if self.selected else (YELLOW if self.hovered else WHITE)
        border_width = 5 if self.selected else 3
        pygame.draw.rect(screen, border_color, self.rect, border_width, border_radius=10)
        
        # Draw world name
        name_surf = self.font.render(self.world_data["name"], True, WHITE)
        name_rect = name_surf.get_rect(center=(self.rect.centerx, self.rect.centery))
        
        # Shadow for text
        shadow_surf = self.font.render(self.world_data["name"], True, BLACK)
        shadow_rect = shadow_surf.get_rect(center=(self.rect.centerx + 2, self.rect.centery + 2))
        screen.blit(shadow_surf, shadow_rect)
        screen.blit(name_surf, name_rect)
        
        # Draw level count
        level_text = f"{self.world_data['levels']} niveaux"
        level_surf = self.font_small.render(level_text, True, WHITE)
        level_rect = level_surf.get_rect(center=(self.rect.centerx, self.rect.bottom - 30))
        shadow_level = self.font_small.render(level_text, True, BLACK)
        shadow_level_rect = shadow_level.get_rect(center=(self.rect.centerx + 1, self.rect.bottom - 29))
        screen.blit(shadow_level, shadow_level_rect)
        screen.blit(level_surf, level_rect)
    
    def update(self, mouse_pos):
        """Update hover state"""
        self.hovered = self.rect.collidepoint(mouse_pos)
    
    def is_clicked(self, pos):
        """Check if card is clicked"""
        return self.rect.collidepoint(pos)


class LevelSelector:
    def __init__(self, screen, worlds_config):
        self.screen = screen
        self.worlds = worlds_config
        self.screen_width = screen.get_width()
        self.screen_height = screen.get_height()
        
        self.selected_world = "Space"  # Default world
        self.selected_level = 1
        self.active = False
        self.view = "WORLDS"  # "WORLDS" or "LEVELS"
        
        # Load world backgrounds
        self.world_backgrounds = {}
        for world_key, world_data in self.worlds.items():
            try:
                # Load the background for each world
                bg_path = f"worlds/{world_data['background']}"
                bg_img = load_image(bg_path, (300, 200))
                self.world_backgrounds[world_key] = bg_img
            except Exception as e:
                print(f"Error loading background for {world_key}: {e}")
                self.world_backgrounds[world_key] = None
        
        self.setup_ui()
    
    def setup_ui(self):
        """Setup UI components"""
        # Load Arcade Classic font
        try:
            arcade_font_path = "assets/fonts/ArcadeClassic/ArcadeClassic.TTF"
            self.font_title = pygame.font.Font(arcade_font_path, 60)
            self.font_medium = pygame.font.Font(arcade_font_path, 35)
            self.font_small = pygame.font.Font(arcade_font_path, 28)
        except:
            self.font_title = pygame.font.Font(None, 60)
            self.font_medium = pygame.font.Font(None, 35)
            self.font_small = pygame.font.Font(None, 28)
        
        # Create world cards
        self.world_cards = []
        card_width = 300
        card_height = 200
        spacing = 40
        
        worlds_list = list(self.worlds.keys())
        
        # Layout: 3 cards on top row (centered), 2 cards on bottom row (centered)
        # Top row: 3 cards
        top_row_cards = worlds_list[:3]
        total_width_top = 3 * card_width + 2 * spacing
        start_x_top = (self.screen_width - total_width_top) // 2
        start_y = 150
        
        for i, world_key in enumerate(top_row_cards):
            x = start_x_top + i * (card_width + spacing)
            y = start_y
            
            card = WorldCard(x, y, card_width, card_height,
                           world_key, self.worlds[world_key],
                           self.world_backgrounds[world_key])
            self.world_cards.append(card)
        
        # Bottom row: 2 cards (centered)
        if len(worlds_list) > 3:
            bottom_row_cards = worlds_list[3:]
            total_width_bottom = len(bottom_row_cards) * card_width + (len(bottom_row_cards) - 1) * spacing
            start_x_bottom = (self.screen_width - total_width_bottom) // 2
            
            for i, world_key in enumerate(bottom_row_cards):
                x = start_x_bottom + i * (card_width + spacing)
                y = start_y + card_height + spacing
                
                card = WorldCard(x, y, card_width, card_height,
                               world_key, self.worlds[world_key],
                               self.world_backgrounds[world_key])
                self.world_cards.append(card)
        
        # Level buttons (will be created dynamically based on selected world)
        self.level_buttons = []
        
        # Back button
        self.back_button = Button(100, self.screen_height - 80,
                                  150, 60, "RETOUR",
                                  self.font_small, bg_color=None, 
                                  text_color=WHITE, border_color=WHITE)
        
        self.create_level_buttons()
    
    def create_level_buttons(self):
        """Create level buttons for selected world as a vertical list"""
        self.level_buttons = []
        
        if self.selected_world not in self.worlds:
            return
        
        num_levels = self.worlds[self.selected_world]["levels"]
        
        # Create vertical list of level buttons
        button_width = 600
        button_height = 60
        spacing = 15
        
        start_x = (self.screen_width - button_width) // 2
        start_y = 180
        
        for i in range(num_levels):
            x = start_x + button_width // 2
            y = start_y + i * (button_height + spacing) + button_height // 2
            
            level_text = f"Niveau {i + 1}"
            
            btn = Button(x, y, button_width, button_height,
                        level_text,
                        self.font_medium,
                        bg_color=None, text_color=WHITE, border_color=WHITE)
            self.level_buttons.append(btn)
        
        # Calculate position for window mode buttons based on last level button
        # Place them between last level button and start button
        last_level_y = start_y + (num_levels - 1) * (button_height + spacing) + button_height
        mode_spacing = 20
        
        # Window mode buttons (windowed, borderless, fullscreen)
        self.window_mode = "windowed"  # default
        mode_width = 160
        mode_height = 30
        total_width = 3 * mode_width + 2 * mode_spacing
        start_x = self.screen_width // 2 - total_width // 2 + mode_width // 2
        mode_y = last_level_y + 25  # 25 pixels below last level button
        
        # Start button - always at bottom of screen
        self.start_button = Button(self.screen_width // 2, 
                                   self.screen_height - 100,
                                   300, 70, "COMMENCER", 
                                   self.font_medium,
                                   bg_color=None, text_color=GREEN, border_color=GREEN, border_width=4)

        self.mode_buttons = {
            "windowed": Button(start_x, mode_y, mode_width, mode_height, "Windowed", self.font_small, bg_color=None, text_color=WHITE, border_color=WHITE),
            "borderless": Button(start_x + (mode_width + mode_spacing), mode_y, mode_width, mode_height, "Borderless", self.font_small, bg_color=None, text_color=WHITE, border_color=WHITE),
            "fullscreen": Button(start_x + 2 * (mode_width + mode_spacing), mode_y, mode_width, mode_height, "Fullscreen", self.font_small, bg_color=None, text_color=WHITE, border_color=WHITE),
        }
    
    def open(self):
        """Open level selector"""
        self.active = True
        self.view = "WORLDS"  # Start with world selection
        # Update selected state for cards
        for card in self.world_cards:
            card.selected = (card.world_name == self.selected_world)
    
    def close(self):
        """Close level selector"""
        self.active = False
    
    def handle_event(self, event):
        """Handle events"""
        if not self.active:
            return None
        
        if event.type == pygame.MOUSEBUTTONDOWN:
            pos = event.pos
            
            if self.view == "WORLDS":
                # World card selection
                for card in self.world_cards:
                    if card.is_clicked(pos):
                        self.selected_world = card.world_name
                        self.selected_level = 1
                        self.create_level_buttons()
                        self.view = "LEVELS"  # Switch to level selection
                        # Update selected state
                        for c in self.world_cards:
                            c.selected = (c.world_name == self.selected_world)
                        return None
                
                # Back button in world view
                if self.back_button.is_clicked(pos):
                    return ("BACK",)
            
            elif self.view == "LEVELS":
                # Level selection
                for i, btn in enumerate(self.level_buttons):
                    if btn.is_clicked(pos):
                        self.selected_level = i + 1
                        print(f"[DEBUG] Level {self.selected_level} selected")
                
                # Start button
                print(f"[DEBUG] Checking start button click at pos {pos}")
                print(f"[DEBUG] Start button rect: {self.start_button.rect}")
                print(f"[DEBUG] Start button enabled: {self.start_button.enabled}")

                # Mode buttons
                for key, btn in self.mode_buttons.items():
                    if btn.is_clicked(pos):
                        self.window_mode = key
                        print(f"[DEBUG] Window mode set to {self.window_mode}")

                if self.start_button.is_clicked(pos):
                    print(f"[DEBUG] Start button clicked! World={self.selected_world}, Level={self.selected_level}, mode={self.window_mode}")
                    return ("START_LEVEL", self.selected_world, self.selected_level, self.window_mode)
                else:
                    print(f"[DEBUG] Start button NOT clicked")
                
                # Back button in level view
                if self.back_button.is_clicked(pos):
                    self.view = "WORLDS"  # Go back to world selection
                    return None
        
        return None
    
    def update(self):
        """Update level selector"""
        if not self.active:
            return
        
        mouse_pos = pygame.mouse.get_pos()
        
        if self.view == "WORLDS":
            # Update world cards
            for card in self.world_cards:
                card.update(mouse_pos)
        
        elif self.view == "LEVELS":
            # Update level buttons
            for btn in self.level_buttons:
                btn.update(mouse_pos)
            
            self.start_button.update(mouse_pos)
        
        self.back_button.update(mouse_pos)
    
    def draw(self, background=None):
        """Draw level selector"""
        if not self.active:
            return
        
        # Draw background
        if background:
            self.screen.blit(background, (0, 0))
        else:
            self.screen.fill((10, 10, 30))
        
        # Semi-transparent overlay
        overlay = pygame.Surface((self.screen_width, self.screen_height), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 150))
        self.screen.blit(overlay, (0, 0))
        
        if self.view == "WORLDS":
            # Draw title
            title = self.font_title.render("CHOISISSEZ VOTRE MONDE", True, CYAN)
            title_rect = title.get_rect(center=(self.screen_width // 2, 60))
            self.screen.blit(title, title_rect)
            
            # Draw world cards
            for card in self.world_cards:
                card.draw(self.screen)
            
            # Draw instruction with border
            instruction = pygame.font.Font(None, 24).render("Cliquez sur un monde pour continuer", True, WHITE)
            inst_rect = instruction.get_rect(center=(self.screen_width // 2, self.screen_height - 70))
            border_rect = inst_rect.inflate(20, 10)
            pygame.draw.rect(self.screen, WHITE, border_rect, 2, border_radius=5)
            
            self.screen.blit(instruction, inst_rect)
        
        elif self.view == "LEVELS":
            # Draw title
            world_name = self.worlds[self.selected_world]["name"]
            title = self.font_title.render(world_name, True, CYAN)
            title_rect = title.get_rect(center=(self.screen_width // 2, 60))
            self.screen.blit(title, title_rect)
            
            # Draw level label
            level_label = self.font_medium.render("CHOISISSEZ UN NIVEAU", True, WHITE)
            level_label_rect = level_label.get_rect(center=(self.screen_width // 2, 140))
            self.screen.blit(level_label, level_label_rect)
            
            # Draw level buttons (list format)
            for i, btn in enumerate(self.level_buttons):
                # Highlight selected level
                if i + 1 == self.selected_level:
                    btn.bg_color = (80, 80, 150)
                    btn.border_color = YELLOW
                    btn.border_width = 5
                    btn.text_color = YELLOW
                else:
                    btn.bg_color = None
                    btn.border_color = WHITE
                    btn.border_width = 3
                    btn.text_color = WHITE
                btn.draw(self.screen)
            
            # Draw start button
            self.start_button.draw(self.screen)

            # Draw window mode buttons
            for key, btn in self.mode_buttons.items():
                if self.window_mode == key:
                    btn.bg_color = (40, 40, 40)
                    btn.border_color = GREEN
                    btn.text_color = GREEN
                    btn.border_width = 4
                else:
                    btn.bg_color = None
                    btn.border_color = WHITE
                    btn.text_color = WHITE
                    btn.border_width = 3
                btn.draw(self.screen)
        
        # Draw back button
        self.back_button.draw(self.screen)
    
    def get_selected_level_info(self):
        """Get info about selected level"""
        return {
            "world": self.selected_world,
            "level": self.selected_level,
            "world_data": self.worlds[self.selected_world]
        }

    def get_selected_mode(self):
        """Returns selected window mode as string: 'windowed'|'borderless'|'fullscreen'"""
        return getattr(self, 'window_mode', 'windowed')
