from django.db import models

class Player(models.Model):
    """Modèle pour les joueurs de SI3LN"""
    username = models.CharField(max_length=50, unique=True)
    email = models.EmailField(unique=True)
    created_at = models.DateTimeField(auto_now_add=True)
    
    class Meta:
        ordering = ['username']
    
    def __str__(self):
        return self.username

class Score(models.Model):
    """Modèle pour les scores des joueurs"""
    player = models.ForeignKey(Player, on_delete=models.CASCADE, related_name='scores')
    value = models.IntegerField()
    level = models.IntegerField(default=1)
    created_at = models.DateTimeField(auto_now_add=True)
    
    class Meta:
        ordering = ['-value']  # Tri décroissant pour le classement
    
    def __str__(self):
        return f"{self.player.username}: {self.value} pts"
