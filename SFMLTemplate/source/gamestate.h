#pragma once

#include <string>

/**
   Singleton to store data that we want to persist between scenes.
   Just a fancy global variable really, but it keeps all this stuff in one place.
 */
class GameState
{
public:
	/** Get the one singleton instance for our game state from anywhere in code. */
    static GameState& getInstance();
    
	/** The path to the selected level from the last time the 'GameSetup' screen was used. */
    std::string selectedLevel = "";
};
