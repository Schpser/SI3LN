#!/bin/bash
# Script de compilation pour SI3LN C++

set -e  # Arrêter en cas d'erreur

echo "========================================"
echo "  SI3LN - Build Script"
echo "========================================"
echo ""

# Vérifier que CMake est installé
if ! command -v cmake &> /dev/null; then
    echo "❌ CMake n'est pas installé!"
    echo "   Ubuntu/Debian: sudo apt-get install cmake"
    echo "   macOS: brew install cmake"
    exit 1
fi

# Vérifier SDL2
if ! pkg-config --exists sdl2; then
    echo "❌ SDL2 n'est pas installé!"
    echo "   Ubuntu/Debian: sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev"
    echo "   macOS: brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer"
    exit 1
fi

echo "✓ CMake trouvé: $(cmake --version | head -n1)"
echo "✓ SDL2 trouvé: $(pkg-config --modversion sdl2)"
echo ""

# Créer et nettoyer le dossier build
echo "📁 Préparation du dossier build..."
rm -rf build
mkdir -p build
cd build

# Configurer avec CMake
echo "⚙️  Configuration CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compiler
echo "🔨 Compilation..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo ""
echo "========================================"
echo "✅ Compilation réussie!"
echo "========================================"
echo ""
echo "Pour lancer le jeu:"
echo "  cd build && ./SI3LN"
echo ""
