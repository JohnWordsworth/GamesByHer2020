#include "game.h"

#include "utils.h"

#include <iostream>
#include <fstream>


const std::string SAVE_PATH = "save_game.txt";


Game& Game::Instance() {
	static Game game;
	return game;
}


void Game::run() {
	std::cout << "Welcome to Awesomeventure!\n";

	while (gameMode != GameMode::None) {	
		switch (gameMode) {
			case GameMode::MainMenu:
				runMainMenu();
				break;

			case GameMode::InGame:
				runGame();
				break;
		}
	}

	std::cout << "Thanks for Playing!\n";
}


void Game::runMainMenu() {
	std::cout << "\n";
	std::cout << "Please select an option...\n";
	std::cout << " [1] Start New Game\n";
	std::cout << " [2] Load Save Game\n";
	std::cout << " [3] Print Debug Information\n";
	std::cout << " [4] Exit Game\n";

	while (true) {
		std::string input;
		std::cout << "> ";
		std::getline(std::cin, input);
		int choice = 0;

		try {
			choice = std::stoi(input);
		} 
		catch (std::exception ex) {
			std::cout << "Invalid Input\n";
			continue;
		}

		if (choice == 1) {
			player = Player();					// Reset Player
			player.currentLocation = gameData.locations[0];
			gameMode = GameMode::InGame;
			return;
		} 
		else if (choice == 2) {
			loadGame(SAVE_PATH);
			gameMode = GameMode::InGame;
			return;
		}
		else if (choice == 3) {
			for (size_t i = 0; i < gameData.locations.size(); ++i) {
				std::cout << gameData.locations[i]->id << ": " << gameData.locations[i]->text << "\n\n";
			}
		}
		else if (choice == 4) {
			return;
		}
	}
}


void Game::runGame() {
	while (true) {
		if (player.currentLocation == nullptr) {
			std::cout << "Your adventure has come to an end.\n";
			gameMode = GameMode::None;
			return;
		}

		std::cout << "\n";
		std::cout << player.currentLocation->text << "\n";

		// First visit - collect any items here!
		if (player.hasVisitedLocation(player.currentLocation->id) == false) {
			player.addVisitedLocation(player.currentLocation->id);

			for (int i = 0; i < player.currentLocation->items.size(); ++i) {
				std::shared_ptr<BaseItem> item = gameData.getItemWithId(player.currentLocation->items[i]);

				if (item != nullptr) {
					std::cout << "* You found: " << item->title << "\n";
					player.addItem(item->id, 1);
				}
			}

			if (player.currentLocation->items.size() > 0) {
				std::cout << "\n";
			}
		}

		if (player.currentLocation->choices.size() == 0) {
			player.currentLocation = nullptr;
			continue;
		}

		for (size_t i = 0; i < player.currentLocation->choices.size(); ++i) {
			std::cout << " [" << (i + 1) << "] " << player.currentLocation->choices[i].text << "\n";
		}

		std::cout << " [m] Menu\n";
		std::cout << " [i] Inventory\n";

		bool validChoice = false;
		std::string input;
		int choice = -1;

		while (validChoice == false) {		
			std::cout << "> ";
			std::getline(std::cin, input);

			if (input.length() == 0) {
				continue;
			}

			if (input[0] == 'm' || input[0] == 'i' || input[0] == 'M' || input[0] == 'I') {
				validChoice = true;
			}
			else {
				try {
					// -1 because we ask the player to enter '1, 2, 3' but the array indices are '0, 1, 2'
					choice = std::stoi(input) - 1;
				}
				catch (std::exception ex) { }

				if (choice >= 0 && choice < (int)player.currentLocation->choices.size()) {
					validChoice = true;
				}
			}
		}

		// If we get here, we know we have a valid input (either 'm' or 'i' in input or a choice index)

		if (input[0] == 'm' || input[0] == 'M') {
			handleInGameMenu();
		}
		else if (input[0] == 'i' || input[0] == 'I') {
			handleInventory();
		}
		else {
			player.currentLocation = gameData.getLocationWithId(player.currentLocation->choices[choice].nextLocationId);
		}
	}
}


void Game::handleInGameMenu() {
	std::cout << "\n";
	std::cout << "Please select an option...\n";
	std::cout << " [1] Continue Game\n";
	std::cout << " [2] Save Game\n";
	std::cout << " [3] Load Last Save\n";
	std::cout << " [4] Exit Game\n";

	while (true) {
		std::string input;
		std::cout << "> ";
		std::getline(std::cin, input);
		int choice = 0;

		try {
			choice = std::stoi(input);
		}
		catch (std::exception ex) {
			std::cout << "Invalid Input\n";
			continue;
		}

		if (choice == 1) {
			gameMode = GameMode::InGame;
			return;
		}
		else if (choice == 2) {
			saveGame(SAVE_PATH);
			gameMode = GameMode::InGame;
			return;
		}
		else if (choice == 3) {
			loadGame(SAVE_PATH);
			gameMode = GameMode::InGame;
			return;
		}
		else if (choice == 4) {
			saveGame(SAVE_PATH);
			gameMode = GameMode::None;
			return;
		}
	}
}


void Game::handleInventory() {
	std::cout << "\n";
	
	if (player.inventory.size() == 0) {
		std::cout << "You have no items in your inventory. Go find some! \n";
		return;
	}
	
	std::cout << "Select an item from your inventory... \n";
	
	for(int i = 0; i < player.inventory.size(); ++i) {
		std::cout << " [" << (i+1) << "] " << player.inventory[i].item->title << "\n";
	}

	while (true) {
		std::string input;
		std::cout << "> ";
		std::getline(std::cin, input);
		int choice = 0;

		try {
			choice = std::stoi(input) - 1;
		}
		catch (std::exception ex) {
			std::cout << "Invalid Input\n";
			continue;
		}

		if (choice >= 0 && choice < player.inventory.size())
		{
			std::shared_ptr<BaseItem> item = player.inventory[choice].item;
			item->useItem();
			
			if (item->consumable) {
				player.removeItem(item->id, 1);
			}

			return;
		}
	}
}


void Game::saveGame(const std::string& path) {
	std::ofstream file(path);

	if (file.is_open() == false) {
		std::cout << "Failed to write save game: " << path << "\n";
		return;
	}

	// Line 1: Player Name
	file << player.name << "\n";

	// Line 2: Current Location ID
	file << player.currentLocation->id << "\n";

	// Line 3: Comma separated list of "itemid:amount" - i.e. "sword:1,apple:2,scroll:1"
	for (int i = 0; i < player.inventory.size(); ++i) {
		if (i > 0) {
			file << ",";
		}

		file << player.inventory[i].item->id << ":" << player.inventory[i].amount;
	}

	file << "\n";

	// Line 4: Comma separated list of visited locations
	for (int i = 0; i < player.visitedLocations.size(); ++i) {
		if (i > 0) {
			file << ",";
		}

		file << player.visitedLocations[i];
	}

	file << "\n";
}


void Game::loadGame(const std::string& path) {
	std::ifstream file(path);

	if (file.is_open() == false) {
		std::cout << "Failed to read save game: " << path << "\n";
		return;
	}

	std::string line;

	// Line 1: Player Name
	std::getline(file, line);
	player.name = line;

	// Line 2: Current Location ID
	std::getline(file, line);
	player.currentLocation = gameData.getLocationWithId(line);

	// Line 3: Inventory
	player.inventory.clear();
	std::getline(file, line);
	std::vector<std::string> items = split(line, ',');

	for (int i = 0; i < items.size(); ++i) {
		std::vector<std::string> tokens = split(items[i], ':');

		if (tokens.size() == 2) {
			InventoryItem entry;
			entry.item = gameData.getItemWithId(tokens[0]);
			entry.amount = std::stoi(tokens[1]);
			player.inventory.push_back(entry);
		}
	}

	// Line 4: Visited Locations
	std::getline(file, line);
	player.visitedLocations = split(line, ',');
}

