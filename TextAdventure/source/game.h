#pragma once

#include "gamedata.h"
#include "player.h"

#include <string>


/**
 * Stores the current 'mode' or state the game is in as a whole.
 */
enum class GameMode {
	None,
	MainMenu,
	InGame,
};



/**
 * Game singleton which stores all of the game data and state for our game.
 **/
class Game {
public:
	static Game& Instance();

	void run();

	GameData gameData;
	Player player;

private:

	void runMainMenu();
	void runGame();

	void handleInGameMenu();
	void handleInventory();

	void saveGame(const std::string& path);
	void loadGame(const std::string& path);

	GameMode gameMode = GameMode::MainMenu;
};