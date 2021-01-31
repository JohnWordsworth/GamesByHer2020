#include "title_scene.h"

#include <iostream>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"

const std::string kTitleMusic = "../assets/music/titlescreen.ogg";
const std::string kTitleScreenFont = "../assets/fonts/orbitron.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kAsteroid01 = "../assets/gfx/asteroid-small-01.png";
const std::string kAsteroid02 = "../assets/gfx/asteroid-small-02.png";
const std::string kAsteroid03 = "../assets/gfx/asteroid-medium-02.png";
const std::string kAsteroid04 = "../assets/gfx/asteroid-medium-03.png";

void TitleScene::onInitializeScene()
{
    m_orbitronFont.loadFromFile(kTitleScreenFont);
    m_titleMusic.openFromFile(kTitleMusic);
    m_titleMusic.setVolume(25);

    std::shared_ptr<gbh::SpriteNode> spriteNode = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    spriteNode->setName("Background");
    addChild(spriteNode);

    std::shared_ptr<gbh::TextNode> textNode = std::make_shared<gbh::TextNode>("SPACE RACE", m_orbitronFont, 60);
    textNode->setPosition(650, 100);
    textNode->setName("Title");
    addChild(textNode);

    std::shared_ptr<gbh::SpriteNode> ship = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    ship->setPosition(640, 300);
    addChild(ship);
    
    m_asteroid01 = std::make_shared<gbh::SpriteNode>(kAsteroid01);
    m_asteroid01->setPosition(300, 500);
    m_asteroid01->setOrigin(0.5f, 0.5f);
    addChild(m_asteroid01);
    
    m_asteroid02 = std::make_shared<gbh::SpriteNode>(kAsteroid02);
    m_asteroid02->setPosition(400, 300);
    m_asteroid02->setOrigin(0.5f, 0.5f);
    addChild(m_asteroid02);
    
    m_asteroid03 = std::make_shared<gbh::SpriteNode>(kAsteroid03);
    m_asteroid03->setPosition(960, 550);
    m_asteroid03->setOrigin(0.5f, 0.5f);
    addChild(m_asteroid03);
    
    m_asteroid04 = std::make_shared<gbh::SpriteNode>(kAsteroid04);
    m_asteroid04->setPosition(200, 160);
    m_asteroid04->setOrigin(0.5f, 0.5f);
    addChild(m_asteroid04);

    std::shared_ptr<gbh::ShapeNode> startButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(200, 60)));
    startButton->setPosition(660, 500);
    startButton->getShape()->setFillColor(sf::Color(0, 255, 0, 64));
    startButton->getShape()->setOutlineThickness(10.f);
    startButton->setName("StartButton");
    addChild(startButton);

    std::shared_ptr<gbh::TextNode> startNode = std::make_shared<gbh::TextNode>("START", m_orbitronFont);
    startNode->setPosition(95, 20);
    startNode->setName("StartButton");
    startButton->addChild(startNode);
    
    std::shared_ptr<gbh::ShapeNode> creditsButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(200, 60)));
    creditsButton->setPosition(660, 590);
    creditsButton->getShape()->setFillColor(sf::Color(0, 255, 0, 64));
    creditsButton->getShape()->setOutlineThickness(10.f);
    creditsButton->setName("CreditsButton");
    addChild(creditsButton);

    std::shared_ptr<gbh::TextNode> creditsNode = std::make_shared<gbh::TextNode>("CREDITS", m_orbitronFont);
    creditsNode->setPosition(95, 20);
    creditsNode->setName("CreditsButton");
    creditsButton->addChild(creditsNode);
}

void TitleScene::onUpdate(double deltaTime)
{
    const float degreesPerSecond = 45.0f;

    m_asteroid01->rotate(degreesPerSecond * deltaTime);
    m_asteroid02->rotate(degreesPerSecond * deltaTime);
    m_asteroid03->rotate(degreesPerSecond * deltaTime);
    m_asteroid04->rotate(degreesPerSecond * deltaTime);
}


void TitleScene::onMouseEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2 mousePosition = { (float)event.mouseButton.x, (float)event.mouseButton.y };
        std::shared_ptr<gbh::Node> node = getNodeAtViewPoint(mousePosition);

        if (node->getName() == "StartButton")
        {
            gbh::Game::getInstance().changeScene("maingame");
        }
        else if (node->getName() == "CreditsButton")
        {
            gbh::Game::getInstance().changeScene("credits");
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
