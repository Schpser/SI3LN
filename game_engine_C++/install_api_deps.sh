#!/bin/bash

echo "🔧 Installing dependencies for API integration..."

# Check if running with sudo
if [ "$EUID" -ne 0 ]; then 
   echo "Please run with sudo: sudo ./install_api_deps.sh"
   exit 1
fi

# Install CURL development library
echo "📦 Installing libcurl..."
apt-get install -y libcurl4-openssl-dev

# Install nlohmann-json library
echo "📦 Installing nlohmann-json..."
apt-get install -y nlohmann-json3-dev

echo "✅ Dependencies installed successfully!"
echo ""
echo "Now you can build the project with: ./build.sh"
