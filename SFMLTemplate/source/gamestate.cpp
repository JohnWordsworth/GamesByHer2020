#include "gamestate.h"


GameState& GameState::getInstance()
{
    static GameState instance;
    return instance;
}
