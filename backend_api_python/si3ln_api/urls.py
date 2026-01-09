from django.contrib import admin
from django.urls import path, include
from rest_framework import routers
from api import views

router = routers.DefaultRouter()
router.register(r'players', views.PlayerViewSet)
router.register(r'scores', views.ScoreViewSet)

urlpatterns = [
    path('admin/', admin.site.urls),
    path('api/', include(router.urls)),
    path('api/health/', views.health_check),
    path('api/auth/', include('rest_framework.urls')),
]
