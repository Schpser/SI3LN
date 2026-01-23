#include "Game.h" // Inclusion du fichier d'en-tête Game.h pour utiliser la classe Game
#include <iostream> // Inclusion de la bibliothèque iostream pour les opérations d'entrée/sortie

int main(int argc, char* argv[]) {
    std::cout << std::endl; // Affichage d'une ligne vide dans le terminal
    std::cout << "   _____ _____ _____ _____ _____ _____ _____ _____ " << std::endl; // Affichage du logo ASCII
    std::cout << "  |  ___|_   _|  ___|_   _|_   _| \\_  |  ___|_   _|" << std::endl; // Affichage de la ligne du logo
    std::cout << "  | |__   | | | |_    | |   | |   | | | |__   | |  " << std::endl; // Affichage de la ligne suivante du logo
    std::cout << "  |  __|  | | |  _|   | |   | |   | | |  __|  | |  " << std::endl; // Affichage de la ligne suivante du logo
    std::cout << "  | |___  | | | |___  | |  _| |_  | | | |___  | |  " << std::endl; // Affichage de la ligne finale du logo
    std::cout << "  |_____| |_| |_____| |_| |_____||_| |_____| |_|  " << std::endl; // Affichage de la dernière ligne du logo
    std::cout << std::endl; // Affichage d'une ligne vide dans le terminal
    
    try {
        SI3LN::Game game; // Création d'une instance de la classe Game
        game.run(); // Appel de la méthode run() pour démarrer le jeu
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl; // Gestion des exceptions et affichage du message d'erreur en cas d'exception levée
        return 1; // Retourne une valeur non nulle pour indiquer qu'une erreur s'est produite
    }
    
    std::cout << std::endl; // Affichage d'une ligne vide dans le terminal
    std::cout << "Thanks for playing!" << std::endl; // Affichage du message de remerciement pour le joueur
    
    return 0; // Retourne 0 pour indiquer que le programme s'est terminé avec succès
}