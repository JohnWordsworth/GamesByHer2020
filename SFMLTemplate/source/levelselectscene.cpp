#include "levelselectscene.h"
#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"
#include <iostream>

const std::string kTitleMusic = "../assets/music/titlescreen.ogg";
const std::string kTitleScreenFont = "../assets/fonts/orbitron.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";

void LevelSelectScene::onInitializeScene()
{
    m_orbitronFont.loadFromFile(kTitleScreenFont);
    m_titleMusic.openFromFile(kTitleMusic);
    m_titleMusic.setVolume(25);

    std::shared_ptr<gbh::SpriteNode> spriteNode = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    spriteNode->setName("Background");
    spriteNode->setPosition(640, 360);
    addChild(spriteNode);

    std::shared_ptr<gbh::TextNode> textNode = std::make_shared<gbh::TextNode>("Select level", m_orbitronFont, 60);
    textNode->setPosition(650, 100);
    textNode->setName("Title");
    addChild(textNode);

    std::shared_ptr<gbh::ShapeNode> startButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(200, 60)));
    startButton->setPosition(660, 500);
    startButton->getShape()->setOutlineThickness(0.5);
    startButton->setName("Level01Button");
    addChild(startButton);

    std::shared_ptr<gbh::TextNode> startNode = std::make_shared<gbh::TextNode>("Level 01", m_orbitronFont);
    startNode->setPosition(95, 20);
    startNode->setName("Level01");
    startButton->addChild(startNode);
    
    std::shared_ptr<gbh::ShapeNode> creditsButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(200, 60)));
    creditsButton->setPosition(660, 590);
    creditsButton->getShape()->setOutlineThickness(0.5);
    creditsButton->setName("Level02Button");
    addChild(creditsButton);

    std::shared_ptr<gbh::TextNode> creditsNode = std::make_shared<gbh::TextNode>("Level 02", m_orbitronFont);
    creditsNode->setPosition(95, 20);
    creditsNode->setName("Level02");
    creditsButton->addChild(creditsNode);
}

void LevelSelectScene::onMouseEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2 mousePosition = { (float)event.mouseButton.x, (float)event.mouseButton.y };
        std::shared_ptr<gbh::Node> node = getNodeAtViewPoint(mousePosition);

        if (node->getName() == "Level01" || node->getName() == "Level01Button")
        {
            m_levelSelected = std::make_shared<int>(1);
            std::cout << "Btn 01 pressed\n";
        }
        else if (node->getName() == "Level02" || node->getName() == "Level02Button")
        {
            m_levelSelected = std::make_shared<int>(2);
            std::cout << "Btn 02 pressed\n";
        }

        gbh::Game::getInstance().changeScene("maingame");
    }
}
