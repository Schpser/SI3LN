#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "
";
    std::cout << "   _____ _____ _____ _____ _____ _____ _____ _____ " << std::endl;
    std::cout << "  |  ___|_   _|  ___|_   _|_   _| \_  |  ___|_   _|
" << std::endl;
    std::cout << "  | |__   | | | |_    | |   | |   | | | |__   | |  " << std::endl;
    std::cout << "  |  __|  | | |  _|   | |   | |   | | |  __|  | |  " << std::endl;
    std::cout << "  | |___  | | | |___  | |  _| |_  | | | |___  | |  " << std::endl;
    std::cout << "  |_____| |_| |_____| |_| |_____||_| |_____| |_|  " << std::endl;
    std::cout << "  _____ _____ _____ _____ _____ _____ _____ _____ " << std::endl;
    std::cout << " |  ___|_   _|  ___|_   _|_   _| \_  |  ___|_   _|
" << std::endl;
    std::cout << " | |__   | | | |_    | |   | |   | | | |__   | |  " << std::endl;
    std::cout << " |  __|  | | |  _|   | |   | |   | | |  __|  | |  " << std::endl;
    std::cout << " | |___  | | | |___  | |  _| |_  | | | |___  | |  " << std::endl;
    std::cout << " |_____| |_| |_____| |_| |_____||_| |_____| |_|  " << std::endl;
    std::cout << " _____ _____ _____ _____ _____ _____ _____ _____ " << std::endl;
    std::cout << "\n" << std::endl;
    std::cout << std::endl;
    
    try {
        SI3LN::Game game("Space Invaders III - Last Night");
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << std::endl;
    std::cout << "Thanks for playing!" << std::endl;
    
    return 0;
}
