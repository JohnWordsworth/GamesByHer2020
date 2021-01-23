#include "title_scene.h"

#include <iostream>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"

const std::string kTitleMusic = "../assets/music/titlescreen.ogg";
const std::string kTitleScreenFont = "../assets/fonts/roboto-regular.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";


void TitleScene::onInitializeScene()
{
	m_robotoFont.loadFromFile(kTitleScreenFont);
	m_titleMusic.openFromFile(kTitleMusic);
	m_titleMusic.setVolume(25);

	std::shared_ptr<gbh::SpriteNode> spriteNode = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
	spriteNode->setName("Background");
	addChild(spriteNode);

	std::shared_ptr<gbh::TextNode> textNode = std::make_shared<gbh::TextNode>("Space Race", m_robotoFont, 60);
    textNode->setOrigin(0.5f, 0.5f);
	textNode->setPosition(640, 100);
	textNode->setName("Title");
	addChild(textNode);

	std::shared_ptr<gbh::SpriteNode> ship = std::make_shared<gbh::SpriteNode>(kPlayerShip);
	ship->setPosition(620, 300);
	addChild(ship);

	std::shared_ptr<gbh::ShapeNode> startButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(1280, 60)));
	startButton->setPosition(640, 560);
    startButton->setOrigin(0.5f, 0.5f);
	startButton->getShape()->setFillColor(sf::Color(0, 255, 0, 64));
	startButton->setName("StartButton");
	addChild(startButton);

	std::shared_ptr<gbh::TextNode> startNode = std::make_shared<gbh::TextNode>("Start Game", m_robotoFont);
	startNode->setPosition(20, 10);
	startNode->setName("StartButton");
	startButton->addChild(startNode);
}


void TitleScene::onUpdate(double deltaTime)
{
    std::shared_ptr<gbh::Node> titleNode = getFirstChildWithName("Title", false);
    
    if (titleNode)
    {
        titleNode->move(0, 50.f * (float)deltaTime);
    }
}


void TitleScene::onMouseEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
        sf::Vector2 clickPosition { (float)event.mouseButton.x, (float)event.mouseButton.y };
        std::shared_ptr<gbh::Node> node = getNodeAtViewPoint(clickPosition);

		if (node->getName() == "StartButton")
		{
			std::cout << "Clicked On: " << node->getName() << "\n";
            gbh::Game::getInstance().changeScene("maingame");
		}
	}
}


void TitleScene::onShowScene()
{
	m_titleMusic.play();
}


void TitleScene::onHideScene()
{
	m_titleMusic.stop();
}
