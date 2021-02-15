#include "gamesetupscene.h"
#include "gamestate.h"

#include <iostream>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"

const std::string kTitleScreenFont = "../assets/fonts/roboto-regular.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";


struct LevelDefinition {
    std::string title;
    std::string file;
};


void GameSetupScene::onInitializeScene()
{
	m_robotoFont.loadFromFile(kTitleScreenFont);

    auto bgNode = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    bgNode->setName("Background");
    bgNode->setPosition(640, 360);
    addChild(bgNode);
    
    auto titleNode = std::make_shared<gbh::TextNode>("Select Stage...", m_robotoFont, 60);
    titleNode->setPosition(640, 80);
    addChild(titleNode);
    
    std::vector<LevelDefinition> levels;
    levels.push_back({ "Level 1", "../assets/levels/level_01.json" });
    levels.push_back({ "Level 2", "../assets/levels/level_02.json" });
    levels.push_back({ "Level 3", "../assets/levels/level_03.json" });
    
    for(int i = 0; i < levels.size(); ++i)
    {
        auto button = createButton(sf::Vector2f(400, 60), levels[i].title, sf::Color(0, 255, 0, 64), "->" + levels[i].file);
        button->setPosition(640.0f, 240 + 80*i);
        addChild(button);
    }

    auto backButton = createButton(sf::Vector2f(400, 60), "Back", sf::Color(0, 0, 255, 64), "Back");
    backButton->setPosition(640, 620);
    addChild(backButton);
}

void GameSetupScene::onMouseEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		std::shared_ptr<gbh::Node> node = this->getNodeAtViewPoint((float)event.mouseButton.x, (float)event.mouseButton.y);

		if (node)
		{
            if (node->getName() == "Back")
            {
                gbh::Game::getInstance().changeScene("title");
            }
                        
            if (node->getName().length() > 4)
            {
                if (node->getName().substr(0, 2) == "->")
                {
                    std::string level = node->getName().substr(2);
                    GameState::getInstance().selectedLevel = level;
                    gbh::Game::getInstance().changeScene("game");
                }
            }
		}
    }
}


std::shared_ptr<gbh::Node> GameSetupScene::createButton(const sf::Vector2f &size, const std::string &text, const sf::Color color, const std::string &name)
{
    auto button = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(size));
    button->getShape()->setFillColor(color);
    button->setName(name);
    
    auto label = std::make_shared<gbh::TextNode>(text, m_robotoFont, 32);
    label->setPosition(size * 0.5f);
    label->setName(name);
    button->addChild(label);
    
    return button;
}
