from django.contrib import admin
from .models import Player, Score

@admin.register(Player)
class PlayerAdmin(admin.ModelAdmin):
    list_display = ('username', 'email', 'created_at')
    search_fields = ('username', 'email')
    ordering = ('-created_at',)

@admin.register(Score)
class ScoreAdmin(admin.ModelAdmin):
    list_display = ('player', 'value', 'level', 'created_at')
    list_filter = ('level', 'created_at')
    search_fields = ('player__username',)
    ordering = ('-value',)
