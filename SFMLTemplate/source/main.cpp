#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "sfml-engine/game.h"

#include "titlescene.h"
#include "gamesetupscene.h"
#include "maingamescene.h"


int main()
{
	gbh::Game& game = gbh::Game::getInstance();

	game.addScene("title", std::make_shared<TitleScene>());
    game.addScene("setup", std::make_shared<GameSetupScene>());
    game.addScene("game", std::make_shared<MainGameScene>());
    
	game.run("title");

	return 0;
}
