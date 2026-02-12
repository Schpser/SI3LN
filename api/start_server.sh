#!/bin/bash
# Script to cleanly start the Django development server

# Kill any existing Django servers
echo "🔍 Checking for running servers..."
if pgrep -f "manage.py runserver" > /dev/null; then
    echo "⚠️  Found running server, stopping it..."
    pkill -f "manage.py runserver"
    sleep 2
fi

# Check if port 8000 is still in use
if lsof -i :8000 > /dev/null 2>&1; then
    echo "⚠️  Port 8000 still in use, forcing cleanup..."
    lsof -ti :8000 | xargs kill -9 2>/dev/null
    sleep 1
fi

echo "🚀 Starting fresh Django server..."
cd "$(dirname "$0")"
python3 manage.py runserver
