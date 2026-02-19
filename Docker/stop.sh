#!/bin/bash

# SI3LN Docker Stop Script

DOCKER_DIR="$(cd "$(dirname "$0")" && pwd)"

echo ""
echo "🛑 Stopping SI3LN containers..."
echo ""

cd "$DOCKER_DIR"

# Stop all containers
docker-compose down

echo ""
echo "✅ All containers stopped!"
echo ""
echo "To start again, run: ./start.sh"
echo "To remove volumes (⚠️  deletes data): docker-compose down -v"
echo ""
