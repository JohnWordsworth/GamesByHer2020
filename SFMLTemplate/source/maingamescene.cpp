#include "maingamescene.h"

#include <iostream>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"
#include "sfml-engine/mathutils.h"


const std::string kGameMusic = "../assets/music/maingame.ogg";
const std::string kGameFont = "../assets/fonts/roboto-regular.ttf";
const std::string kStarfield = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";


void MainGameScene::onInitializeScene()
{
    m_robotoFont.loadFromFile(kGameFont);
    m_gameMusic.openFromFile(kGameMusic);
    m_gameMusic.setVolume(25);
    
    createPhysicsWorld(sf::Vector2f());
    setDrawPhysicsDebug(true);
    
    std::shared_ptr<gbh::SpriteNode> bgNode = std::make_shared<gbh::SpriteNode>(kStarfield);
    bgNode->setName("background");
    bgNode->setPosition(640, 360);
    addChild(bgNode);
    
    std::shared_ptr<gbh::Node> worldBoundary = std::make_shared<gbh::Node>();
    worldBoundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(1240, 700)));
    worldBoundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
    worldBoundary->setPosition(640, 360);
    addChild(worldBoundary);
    
    m_ship = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_ship->setPosition(640, 360);
    m_ship->setOrigin(0.5f, 0.5f);
    m_ship->setScale(0.5f, 0.5f);
    m_ship->setPhysicsBody(getPhysicsWorld()->createBox(sf::Vector2f(40.f, 60.f)));
    m_ship->getPhysicsBody()->setType(gbh::PhysicsBodyType::Dynamic);
    m_ship->getPhysicsBody()->setLinearDamping(2.0f);
    m_ship->getPhysicsBody()->setFixedRotation(true);
    addChild(m_ship);
}


void MainGameScene::onUpdate(double deltaTime)
{
    //simpleMovementUpdate(deltaTime);
    advancedMovementUpdate(deltaTime);
}


void MainGameScene::simpleMovementUpdate(double deltaTime)
{
    const float acceleration = 2000.0f;
    sf::Vector2f moveDirection;

    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::W))     { moveDirection.y -= 1.0f; }
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::S))     { moveDirection.y += 1.0f; }
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::A))     { moveDirection.x -= 1.0f; }
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::D))     { moveDirection.x += 1.0f; }
    
    moveDirection = gbh::math::normalize(moveDirection);
    m_ship->getPhysicsBody()->applyForceToCenter(moveDirection * acceleration);
}


void MainGameScene::advancedMovementUpdate(double deltaTime)
{
    const float degreesPerSecond = 180.0f;
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::A))
    {
        m_ship->rotate(-(float)deltaTime * degreesPerSecond);
    }
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::D))
    {
        m_ship->rotate((float)deltaTime * degreesPerSecond);
    }
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::W))
    {
        const float acceleration = 2000.0f;
        sf::Vector2f moveDirection = m_ship->forwardVector();
        m_ship->getPhysicsBody()->applyForceToCenter(moveDirection * acceleration);
    }

}


void MainGameScene::onMouseEvent(sf::Event &event)
{
    
}


void MainGameScene::onKeyboardEvent(sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::U)
        {
            setDrawPhysicsDebug(!getDrawPhysicsDebug());
        }
    }
}


void MainGameScene::onJoystickEvent(sf::Event &event)
{
    
}


void MainGameScene::onShowScene()
{
	m_gameMusic.play();
}


void MainGameScene::onHideScene()
{
	m_gameMusic.stop();
}
