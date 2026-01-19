#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "  SI3LN - Space Invaders III Last Night" << std::endl;
    std::cout << "  C++ Version with SDL2" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    try {
        SI3LN::Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "Thanks for playing!" << std::endl;
    
    return 0;
}
