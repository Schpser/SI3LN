from rest_framework import viewsets, status
from rest_framework.decorators import api_view, action
from rest_framework.response import Response
from .models import Player, Score
from .serializers import PlayerSerializer, ScoreSerializer

@api_view(['GET'])
def health_check(request):
    """Endpoint de santé de l'API"""
    return Response({
        "status": "ok",
        "service": "SI3LN Gaming API",
        "version": "1.0.0",
        "endpoints": {
            "players": "/api/players/",
            "scores": "/api/scores/",
            "leaderboard": "/api/scores/?limit=10"
        }
    })

class PlayerViewSet(viewsets.ModelViewSet):
    """ViewSet pour la gestion des joueurs"""
    queryset = Player.objects.all()
    serializer_class = PlayerSerializer
    
    @action(detail=True, methods=['get'])
    def scores(self, request, pk=None):
        """Récupère tous les scores d'un joueur spécifique"""
        player = self.get_object()
        scores = player.scores.all().order_by('-value')
        serializer = ScoreSerializer(scores, many=True)
        return Response(serializer.data)

class ScoreViewSet(viewsets.ModelViewSet):
    """ViewSet pour la gestion des scores"""
    serializer_class = ScoreSerializer
    
    # AJOUTE CE QUERYSET DE BASE
    queryset = Score.objects.all()
    
    def get_queryset(self):
        """Retourne les scores avec possibilité de filtrage"""
        queryset = Score.objects.all().order_by('-value')
        
        # Filtre par joueur si player_id est fourni
        player_id = self.request.query_params.get('player_id')
        if player_id:
            queryset = queryset.filter(player_id=player_id)
        
        # Limite pour le classement
        limit = self.request.query_params.get('limit')
        if limit:
            try:
                queryset = queryset[:int(limit)]
            except ValueError:
                pass
        
        return queryset
    
    def create(self, request):
        """Crée un nouveau score"""
        serializer = self.get_serializer(data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data, status=status.HTTP_201_CREATED)
        return Response(serializer.errors, status=status.HTTP_400_BAD_REQUEST)
