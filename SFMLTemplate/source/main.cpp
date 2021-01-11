#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include "sfml-engine/game.h"

#include "title_scene.h"


int main()
{
	gbh::Game& game = gbh::Game::getInstance();

	game.addScene("title", std::make_shared<TitleScene>());
	game.run("title");

	return 0;
}
