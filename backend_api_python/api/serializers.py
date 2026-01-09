from rest_framework import serializers
from .models import Player, Score

class PlayerSerializer(serializers.ModelSerializer):
    """Serializer pour les joueurs"""
    class Meta:
        model = Player
        fields = ['id', 'username', 'email', 'created_at']
        read_only_fields = ['created_at']

class ScoreSerializer(serializers.ModelSerializer):
    """Serializer pour les scores avec nom du joueur"""
    player_name = serializers.CharField(source='player.username', read_only=True)
    
    class Meta:
        model = Score
        fields = ['id', 'player', 'player_name', 'value', 'level', 'created_at']
        read_only_fields = ['created_at']
