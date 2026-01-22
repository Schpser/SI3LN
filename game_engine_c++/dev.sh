#!/bin/bash

# SI3LN Game Engine - Development Helper Script
# This script provides convenient commands for development

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Functions
print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}  SI3LN Game Engine - Dev Helper${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo ""
}

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_error() {
    echo -e "${RED}✗ $1${NC}"
}

print_info() {
    echo -e "${YELLOW}→ $1${NC}"
}

show_help() {
    print_header
    echo "Usage: ./dev.sh [command]"
    echo ""
    echo "Commands:"
    echo "  build         - Build the project"
    echo "  run           - Build and run the game"
    echo "  clean         - Clean build files"
    echo "  rebuild       - Clean and rebuild"
    echo "  debug         - Build with debug symbols"
    echo "  release       - Build optimized release"
    echo "  test          - Run tests (if available)"
    echo "  format        - Format code (requires clang-format)"
    echo "  check         - Check code quality"
    echo "  stats         - Show project statistics"
    echo "  help          - Show this help message"
    echo ""
}

cmd_build() {
    print_info "Building project..."
    make
    print_success "Build complete!"
}

cmd_run() {
    print_info "Building and running..."
    make run
}

cmd_clean() {
    print_info "Cleaning build files..."
    make clean
    print_success "Clean complete!"
}

cmd_rebuild() {
    print_info "Rebuilding project..."
    make rebuild
    print_success "Rebuild complete!"
}

cmd_debug() {
    print_info "Building debug version..."
    make debug
    print_success "Debug build complete!"
}

cmd_release() {
    print_info "Building release version..."
    make release
    print_success "Release build complete!"
}

cmd_test() {
    print_info "Running tests..."
    if [ -f "bin/game_engine" ]; then
        ./bin/game_engine --test
    else
        print_error "Executable not found. Build first."
        exit 1
    fi
}

cmd_format() {
    print_info "Formatting code..."
    if command -v clang-format &> /dev/null; then
        find src include -name "*.cpp" -o -name "*.hpp" | xargs clang-format -i
        clang-format -i main.cpp
        print_success "Code formatted!"
    else
        print_error "clang-format not found. Install it first."
        exit 1
    fi
}

cmd_check() {
    print_info "Checking code quality..."
    
    # Check for common issues
    echo "Checking for TODO comments..."
    grep -rn "TODO" src include main.cpp || echo "  No TODOs found"
    
    echo ""
    echo "Checking for FIXME comments..."
    grep -rn "FIXME" src include main.cpp || echo "  No FIXMEs found"
    
    print_success "Check complete!"
}

cmd_stats() {
    print_header
    print_info "Project Statistics"
    echo ""
    
    echo "Files:"
    echo "  Headers: $(find include -name "*.hpp" | wc -l)"
    echo "  Sources: $(find src -name "*.cpp" | wc -l)"
    echo "  Total: $(find include src -name "*.cpp" -o -name "*.hpp" | wc -l)"
    echo ""
    
    echo "Lines of Code:"
    echo "  Headers: $(find include -name "*.hpp" -exec wc -l {} + | tail -1 | awk '{print $1}')"
    echo "  Sources: $(find src -name "*.cpp" -exec wc -l {} + | tail -1 | awk '{print $1}')"
    echo "  Main: $(wc -l main.cpp | awk '{print $1}')"
    echo ""
    
    if [ -f "bin/game_engine" ]; then
        echo "Executable:"
        ls -lh bin/game_engine | awk '{print "  Size: " $5}'
    fi
    
    print_success "Statistics complete!"
}

# Main script
case "${1:-help}" in
    build)
        cmd_build
        ;;
    run)
        cmd_run
        ;;
    clean)
        cmd_clean
        ;;
    rebuild)
        cmd_rebuild
        ;;
    debug)
        cmd_debug
        ;;
    release)
        cmd_release
        ;;
    test)
        cmd_test
        ;;
    format)
        cmd_format
        ;;
    check)
        cmd_check
        ;;
    stats)
        cmd_stats
        ;;
    help|--help|-h)
        show_help
        ;;
    *)
        print_error "Unknown command: $1"
        echo ""
        show_help
        exit 1
        ;;
esac
