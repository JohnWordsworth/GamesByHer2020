#include "levelselectscene.h"
#include "gamestate.h"

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
    textNode->setName("LevelSelect");
    addChild(textNode);

    std::shared_ptr<gbh::ShapeNode> level01Btn = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(200, 60)));
    level01Btn->setPosition(660, 500);
    level01Btn->getShape()->setOutlineThickness(0.5);
    level01Btn->getShape()->setFillColor(sf::Color(0, 255, 0, 1.0));
    level01Btn->setName("Level01Button");
    addChild(level01Btn);

    std::shared_ptr<gbh::TextNode> level01Txt = std::make_shared<gbh::TextNode>("Level 01", m_orbitronFont);
    level01Txt->setPosition(95, 20);
    level01Txt->setName("Level01");
    level01Btn->addChild(level01Txt);
    
    std::shared_ptr<gbh::ShapeNode> level02Btn = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(200, 60)));
    level02Btn->setPosition(660, 590);
    level02Btn->getShape()->setFillColor(sf::Color(0, 255, 0, 1.0));
    level02Btn->getShape()->setOutlineThickness(0.5);
    level02Btn->setName("Level02Button");
    addChild(level02Btn);

    std::shared_ptr<gbh::TextNode> level02Txt = std::make_shared<gbh::TextNode>("Level 02", m_orbitronFont);
    level02Txt->setPosition(95, 20);
    level02Txt->setName("Level02");
    level02Btn->addChild(level02Txt);
}

void LevelSelectScene::onMouseEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2 mousePosition = { (float)event.mouseButton.x, (float)event.mouseButton.y };
        std::shared_ptr<gbh::Node> node = getNodeAtViewPoint(mousePosition);

        if (node->getName() == "Level01" || node->getName() == "Level01Button")
        {
            GameState::getInstance().selectedLevel = "../assets/json/level01.json";
            std::cout << "Btn 01 pressed\n";
        }
        else if (node->getName() == "Level02" || node->getName() == "Level02Button")
        {
            GameState::getInstance().selectedLevel = "../assets/json/level02.json";
            std::cout << "Btn 02 pressed\n";
        }

        gbh::Game::getInstance().changeScene("maingame");
    }
}
