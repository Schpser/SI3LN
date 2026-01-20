"""
Game entities: Player, Enemy, Bullet
"""
import pygame
import random
from constants import *


class Player:
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.width = 32
        self.height = 32
        
    def to_dict(self):
        """Pour sérialiser en JSON (pour l'API)"""
        return {
            "x": self.x,
            "y": self.y,
            "width": self.width,
            "height": self.height
        }


class Enemy:
    """Enemy entity"""
    def __init__(self, x, y, screen_width, level=1):
        self.x = x
        self.y = y
        self.width = 32
        self.height = 32
        self.screen_width = screen_width
        
        # Movement
        self.speed = ENEMY_SPEED + (level * 0.2)
        self.direction = 1
        self.drop_distance = 20
        
        # Shooting
        self.last_shot = 0
        self.shoot_cooldown = max(700, 3000 - level * 100)
        self.shoot_chance = min(0.1 * level, 0.5)
        
        # Boundaries
        self.min_x = 20
        self.max_x = screen_width - 20
        self.max_y = screen_width // 2
    
    def update(self, current_time):
        """Update enemy position"""
        # Horizontal movement
        self.x += self.speed * self.direction
        
        # Boundary checking and direction change
        if self.x + self.width >= self.max_x and self.direction > 0:
            self.direction = -1
            self.y += self.drop_distance
        elif self.x <= self.min_x and self.direction < 0:
            self.direction = 1
            self.y += self.drop_distance
        
        # Keep within vertical bounds
        if self.y + self.height > self.max_y:
            self.y = self.max_y - self.height
    
    def can_shoot(self, current_time):
        """Check if enemy can shoot"""
        if current_time - self.last_shot > self.shoot_cooldown:
            if random.random() < self.shoot_chance:
                self.last_shot = current_time
                return True
        return False
    
    def to_dict(self):
        """Serialize to JSON"""
        return {
            "x": self.x,
            "y": self.y,
            "width": self.width,
            "height": self.height
        }


class Bullet:
    """Bullet entity"""
    def __init__(self, x, y, is_player_bullet, screen_height):
        self.x = x
        self.y = y
        self.width = 4
        self.height = 10
        self.is_player_bullet = is_player_bullet
        self.screen_height = screen_height
        
        if is_player_bullet:
            self.speed = -PLAYER_BULLET_SPEED
        else:
            self.speed = ENEMY_BULLET_SPEED
        
        self.active = True
    
    def update(self):
        """Update bullet position"""
        self.y += self.speed
        
        # Remove if off screen
        if self.y + self.height < 0 or self.y > self.screen_height:
            self.active = False
    
    def to_dict(self):
        """Serialize to JSON"""
        return {
            "x": self.x,
            "y": self.y,
            "is_player_bullet": self.is_player_bullet
        }


class Explosion:
    """Simple explosion effect"""
    def __init__(self, x, y):
        self.x = x
        self.y = y
        self.current_frame = 0
        self.max_frames = 18  # 6 sizes * 3 animation_speed
        self.animation_speed = 3
        self.counter = 0
        self.active = True
        self.sizes = [10, 20, 30, 25, 15, 5]
    
    def update(self):
        """Update explosion animation"""
        self.counter += 1
        if self.counter >= self.animation_speed:
            self.counter = 0
            self.current_frame += 1
            
            if self.current_frame >= len(self.sizes):
                self.active = False
    
    def get_size(self):
        """Get current explosion size"""
        if self.current_frame < len(self.sizes):
            return self.sizes[self.current_frame]
        return 0
    
    def to_dict(self):
        """Serialize to JSON"""
        return {
            "x": self.x,
            "y": self.y,
            "size": self.get_size()
        }


class PowerUp:
    """Power-up collectible"""
    def __init__(self, x, y, power_type="health"):
        self.x = x
        self.y = y
        self.width = 30
        self.height = 30
        self.power_type = power_type
        self.speed = 2
        self.active = True
    
    def update(self):
        """Move power-up down"""
        self.y += self.speed
        if self.y > 800:
            self.active = False
    
    def to_dict(self):
        """Serialize to JSON"""
        return {
            "x": self.x,
            "y": self.y,
            "power_type": self.power_type
        }


class Bonus:
    """Bonus collectible"""
    def __init__(self, x, y, bonus_type):
        self.x = x
        self.y = y
        self.width = 30
        self.height = 30
        self.bonus_type = bonus_type
        self.speed = 3
        self.active = True
    
    def update(self, screen_height):
        """Move bonus down"""
        self.y += self.speed
        if self.y > screen_height:
            self.active = False
    
    def to_dict(self):
        """Serialize to JSON"""
        return {
            "x": self.x,
            "y": self.y,
            "bonus_type": self.bonus_type
        }


class SpecialAttack:
    """Special attack entity"""
    def __init__(self, attack_type, world, level, screen_width, screen_height):
        self.attack_type = attack_type
        self.world = world
        self.level = level
        self.screen_width = screen_width
        self.screen_height = screen_height
        self.active = True
        
        if world == "Space":
            self.x = random.randint(50, screen_width - 50)
            self.y = 0
            self.width = 10
            self.height = screen_height
            self.speed = 5
            self.speed_x = 0
            self.speed_y = self.speed
            
        elif world == "Desert":
            self.x = 0
            self.y = 0
            self.width = screen_width
            self.height = screen_height
            
        elif world == "Forest":
            self.x = screen_width // 2
            self.y = screen_height - 50
            self.width = 80
            self.height = 80
            
        elif world == "Marine":
            self.x = random.randint(50, screen_width - 50)
            self.y = 0
            self.width = 40
            self.height = 40
            self.speed = 3
            self.speed_x = 0
            self.speed_y = self.speed
            
        elif world == "Apocalyptic":
            self.x = random.randint(50, screen_width - 50)
            self.y = 0
            self.width = 50
            self.height = 50
            self.speed_x = random.choice([-3, 3])
            self.speed_y = 4
            self.bounces = 0
            self.max_bounces = 3
    
    def update(self):
        """Update special attack"""
        if self.world == "Space":
            self.y += self.speed_y
            if self.y > self.screen_height:
                self.active = False
                
        elif self.world == "Marine":
            self.y += self.speed_y
            if self.y > self.screen_height:
                self.active = False
                
        elif self.world == "Apocalyptic":
            self.x += self.speed_x
            self.y += self.speed_y
            
            if self.x <= 0 or self.x + self.width >= self.screen_width:
                self.speed_x = -self.speed_x
                self.bounces += 1
            if self.y <= 0:
                self.speed_y = -self.speed_y
                self.bounces += 1
                
            if self.bounces >= self.max_bounces or self.y > self.screen_height:
                self.active = False
    
    def to_dict(self):
        """Serialize to JSON"""
        return {
            "x": self.x,
            "y": self.y,
            "width": self.width,
            "height": self.height,
            "world": self.world,
            "attack_type": self.attack_type
        }
