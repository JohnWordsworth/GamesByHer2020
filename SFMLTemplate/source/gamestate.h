#pragma once

#include <string>

class GameState
{
public:
    static GameState& getInstance();
    
    std::string selectedLevel = "";
};
