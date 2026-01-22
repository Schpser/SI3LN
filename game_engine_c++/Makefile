# SI3LN Game Engine - Makefile
# C++ compilation configuration

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
LDFLAGS = -lpthread

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/game_engine

# Source files
ENTITY_SOURCES = $(wildcard $(SRC_DIR)/entities/*.cpp)
GAME_SOURCES = $(wildcard $(SRC_DIR)/game/*.cpp)
UTILS_SOURCES = $(wildcard $(SRC_DIR)/utils/*.cpp)
MAIN_SOURCE = main.cpp

ALL_SOURCES = $(ENTITY_SOURCES) $(GAME_SOURCES) $(UTILS_SOURCES) $(MAIN_SOURCE)

# Object files
ENTITY_OBJECTS = $(patsubst $(SRC_DIR)/entities/%.cpp,$(BUILD_DIR)/entities/%.o,$(ENTITY_SOURCES))
GAME_OBJECTS = $(patsubst $(SRC_DIR)/game/%.cpp,$(BUILD_DIR)/game/%.o,$(GAME_SOURCES))
UTILS_OBJECTS = $(patsubst $(SRC_DIR)/utils/%.cpp,$(BUILD_DIR)/utils/%.o,$(UTILS_SOURCES))
MAIN_OBJECT = $(BUILD_DIR)/main.o

ALL_OBJECTS = $(ENTITY_OBJECTS) $(GAME_OBJECTS) $(UTILS_OBJECTS) $(MAIN_OBJECT)

# Colors for pretty output
COLOR_RESET = \033[0m
COLOR_BOLD = \033[1m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_BLUE = \033[34m

# Default target
all: directories $(TARGET)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Build complete!$(COLOR_RESET)"
	@echo "$(COLOR_BLUE)Executable: $(TARGET)$(COLOR_RESET)"

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)/entities
	@mkdir -p $(BUILD_DIR)/game
	@mkdir -p $(BUILD_DIR)/utils
	@mkdir -p $(BIN_DIR)

# Link executable
$(TARGET): $(ALL_OBJECTS)
	@echo "$(COLOR_YELLOW)Linking $@...$(COLOR_RESET)"
	@$(CXX) $(ALL_OBJECTS) -o $@ $(LDFLAGS)

# Compile entity sources
$(BUILD_DIR)/entities/%.o: $(SRC_DIR)/entities/%.cpp
	@echo "$(COLOR_BLUE)Compiling $<...$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile game sources
$(BUILD_DIR)/game/%.o: $(SRC_DIR)/game/%.cpp
	@echo "$(COLOR_BLUE)Compiling $<...$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile utils sources
$(BUILD_DIR)/utils/%.o: $(SRC_DIR)/utils/%.cpp
	@echo "$(COLOR_BLUE)Compiling $<...$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main
$(BUILD_DIR)/main.o: $(MAIN_SOURCE)
	@echo "$(COLOR_BLUE)Compiling $<...$(COLOR_RESET)"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	@echo "$(COLOR_YELLOW)Cleaning build files...$(COLOR_RESET)"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "$(COLOR_GREEN)Clean complete!$(COLOR_RESET)"

# Rebuild everything
rebuild: clean all

# Run the game
run: all
	@echo "$(COLOR_GREEN)Running game engine...$(COLOR_RESET)"
	@./$(TARGET)

# Install (copy to system location - optional)
install: all
	@echo "$(COLOR_YELLOW)Installing game engine...$(COLOR_RESET)"
	@cp $(TARGET) /usr/local/bin/
	@echo "$(COLOR_GREEN)Installation complete!$(COLOR_RESET)"

# Uninstall
uninstall:
	@echo "$(COLOR_YELLOW)Uninstalling game engine...$(COLOR_RESET)"
	@rm -f /usr/local/bin/game_engine
	@echo "$(COLOR_GREEN)Uninstallation complete!$(COLOR_RESET)"

# Debug build (with debug symbols)
debug: CXXFLAGS += -g -DDEBUG
debug: clean all
	@echo "$(COLOR_GREEN)Debug build complete!$(COLOR_RESET)"

# Release build (optimized)
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all
	@echo "$(COLOR_GREEN)Release build complete!$(COLOR_RESET)"

# Show help
help:
	@echo "$(COLOR_BOLD)SI3LN Game Engine - Makefile Help$(COLOR_RESET)"
	@echo ""
	@echo "$(COLOR_YELLOW)Available targets:$(COLOR_RESET)"
	@echo "  $(COLOR_GREEN)all$(COLOR_RESET)       - Build the game engine (default)"
	@echo "  $(COLOR_GREEN)clean$(COLOR_RESET)     - Remove all build files"
	@echo "  $(COLOR_GREEN)rebuild$(COLOR_RESET)   - Clean and rebuild everything"
	@echo "  $(COLOR_GREEN)run$(COLOR_RESET)       - Build and run the game engine"
	@echo "  $(COLOR_GREEN)debug$(COLOR_RESET)     - Build with debug symbols"
	@echo "  $(COLOR_GREEN)release$(COLOR_RESET)   - Build optimized release version"
	@echo "  $(COLOR_GREEN)install$(COLOR_RESET)   - Install to /usr/local/bin"
	@echo "  $(COLOR_GREEN)uninstall$(COLOR_RESET) - Remove from /usr/local/bin"
	@echo "  $(COLOR_GREEN)help$(COLOR_RESET)      - Show this help message"
	@echo ""

# Phony targets
.PHONY: all directories clean rebuild run install uninstall debug release help
