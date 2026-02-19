#!/bin/bash

# SI3LN Docker Quick Start Script
# This script helps you quickly start the entire SI3LN application

set -e

DOCKER_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(dirname "$DOCKER_DIR")"

echo ""
echo "╔════════════════════════════════════════════════════╗"
echo "║                                                    ║"
echo "║   🚀 SI3LN - Space Invaders III Last Night 🚀    ║"
echo "║                                                    ║"
echo "║         Docker Multi-Container Setup               ║"
echo "║                                                    ║"
echo "╚════════════════════════════════════════════════════╝"
echo ""

# Check if docker is installed
if ! command -v docker &> /dev/null; then
    echo "❌ Docker is not installed. Please install Docker first."
    exit 1
fi

# Check if docker-compose is installed
if ! command -v docker-compose &> /dev/null; then
    echo "❌ Docker Compose is not installed. Please install Docker Compose first."
    exit 1
fi

cd "$DOCKER_DIR"

echo "📦 Starting SI3LN containers..."
echo ""

# Build and start containers
docker-compose up -d --build

echo ""
echo "⏳ Waiting for services to be healthy..."
sleep 5

# Check service status
echo ""
echo "📊 Service Status:"
docker-compose ps

echo ""
echo "╔════════════════════════════════════════════════════╗"
echo "║              🎉 Setup Complete! 🎉                 ║"
echo "╚════════════════════════════════════════════════════╝"
echo ""
echo "🌐 Access Points:"
echo ""
echo "   Frontend:      http://localhost"
echo "   API Docs:      http://localhost:8000/api/docs"
echo "   Admin Panel:   http://localhost:8000/admin"
echo ""
echo "🔐 Admin Credentials:"
echo "   Username: admin"
echo "   Password: admin123"
echo ""
echo "📝 Useful Commands:"
echo ""
echo "   View logs:        docker-compose logs -f"
echo "   Stop services:    docker-compose down"
echo "   Restart:          docker-compose restart"
echo "   Shell access:     docker-compose exec api bash"
echo ""
echo "📚 Full documentation: $DOCKER_DIR/README.md"
echo ""
echo "Happy coding! 🎮"
echo ""
