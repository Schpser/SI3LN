#include "Game.h"	// Inclusion du fichier d'en-tête Game.h pour utiliser la classe Game
#include <iostream> // Inclusion de la bibliothèque iostream pour les opérations d'entrée/sortie
#include <string>   // Inclusion pour std::string
#include <algorithm> // Inclusion pour std::transform et std::tolower

int main(int argc, char *argv[])
{
	std::cout << std::endl;															  // Affichage d'une ligne vide dans le terminal
	std::cout << "   _____ _____ _____ _____ _____ _____ _____ _____ " << std::endl;  // Affichage du logo ASCII
	std::cout << "  |  ___|_   _|  ___|_   _|_   _| \\_  |  ___|_   _|" << std::endl; // Affichage de la ligne du logo
	std::cout << "  | |__   | | | |_    | |   | |   | | | |__   | |  " << std::endl;  // Affichage de la ligne suivante du logo
	std::cout << "  |  __|  | | |  _|   | |   | |   | | |  __|  | |  " << std::endl;  // Affichage de la ligne suivante du logo
	std::cout << "  | |___  | | | |___  | |  _| |_  | | | |___  | |  " << std::endl;  // Affichage de la ligne finale du logo
	std::cout << "  |_____| |_| |_____| |_| |_____| |_| |_____| |_|  " << std::endl;	  // Affichage de la dernière ligne du logo
	std::cout << std::endl;															  // Affichage d'une ligne vide dans le terminal

	// Parse command line arguments (passed from Python UI)
	std::string world = "Space";  // Default world
	int level = 1;                // Default level
	int playerIndex = 0; // Default player index
	
	if (argc >= 2)
	{
		world = argv[1];
		std::cout << "World selected: " << world << std::endl;
	}
	if (argc >= 3)
	{
		try {
			level = std::stoi(argv[2]);
			std::cout << "Level selected: " << level << std::endl;
		} catch (...) {
			level = 1;
		}
	}
	if (argc >= 4) {
        try {
            playerIndex = std::stoi(argv[3]);
            std::cout << "Player selected: " << playerIndex << std::endl;
        } catch (...) {
            playerIndex = 0;
        }
    }

	try
	{
		SI3LN::Game game; // Création d'une instance de la classe Game
		game.setWorldAndLevel(world, level);  // Set world and level from Python UI
		game.setPlayerIndex(playerIndex); // Index du joueur

		// Optionnel: lire un 5ème argument pour le mode de la fenêtre: windowed, borderless, fullscreen
		if (argc >= 5) {
			std::string modeArg = argv[4];
			std::transform(modeArg.begin(), modeArg.end(), modeArg.begin(), [](unsigned char c){ return std::tolower(c); });
			if (modeArg == "windowed") {
				game.setWindowMode(SI3LN::Game::WindowMode::WINDOWED);
			} else if (modeArg == "borderless" || modeArg == "borderless_windowed") {
				game.setWindowMode(SI3LN::Game::WindowMode::BORDERLESS);
			} else if (modeArg == "fullscreen") {
				game.setWindowMode(SI3LN::Game::WindowMode::FULLSCREEN);
			}
		}
		game.run();		  // Appel de la méthode run() pour démarrer le jeu
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl; // Gestion des exceptions et affichage du message d'erreur en cas d'exception levée
		return 1;										 // Retourne une valeur non nulle pour indiquer qu'une erreur s'est produite
	}

	std::cout << std::endl;							 // Affichage d'une ligne vide dans le terminal
	std::cout << "Thanks for playing!" << std::endl; // Affichage du message de remerciement pour le joueur

	return 0; // Retourne 0 pour indiquer que le programme s'est terminé avec succès
}
