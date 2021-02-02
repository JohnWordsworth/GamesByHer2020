#include "maingamescene.h"
#include "sfml-engine/game.h"
#include "sfml-engine/mathutils.h"
#include <iostream>

const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kAsteroid01 = "../assets/gfx/asteroid-small-01.png";

void MainGameScene::onInitializeScene() {
    std::shared_ptr<gbh::SpriteNode> spriteMainBg = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    spriteMainBg->setName("GameBackground");
    addChild(spriteMainBg);

    //Add initial physics and player ship
    createPhysicsWorld(sf::Vector2f(0.0f,0.0f));

    // Add a boundary that is almost as big as the screen (1270, 710) and centered.
    std::shared_ptr<gbh::Node> boundary = std::make_shared<gbh::Node>();
    boundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(1270, 710)));
    boundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
    boundary->setPosition(1280.0f/2.0f, 720.0f/2.0f);
    addChild(boundary);
                             
    const sf::Vector2f shipSize = sf::Vector2f(80.0f, 120.0f);
    
    m_playerShip = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_playerShip->setName("playership");
    m_playerShip->setPosition(620, 300);
    m_playerShip->setScale(0.5f, 0.5f);
    m_playerShip->setPhysicsBody(getPhysicsWorld()->createBox(shipSize * 0.5f));
    m_playerShip->getPhysicsBody()->setLinearDamping(2.0f);
    m_playerShip->getPhysicsBody()->setFixedRotation(true);
    addChild(m_playerShip);
    
    //Add obstacle asteroid
    m_asteroidObstacle01 = std::make_shared<gbh::SpriteNode>(kAsteroid01);
    m_asteroidObstacle01->setPosition(300, 500);
    m_asteroidObstacle01->setOrigin(0.5f, 0.5f);
    
    std::shared_ptr<gbh::PhysicsBody> body = getPhysicsWorld()->createBox(sf::Vector2f(20.f, 20.0f));
    body->setType(gbh::PhysicsBodyType::Static);
    body->setLinearDamping(0.1f);

    m_asteroidObstacle01->setPhysicsBody(body);
    addChild(m_asteroidObstacle01);

}

void MainGameScene::onUpdate(double deltaTime)
{
    sf::Vector2f moveDirection;
    const float accelerationForce = 2000.0f;
    const float degreesPerSecond = 45.0f;

    //would be sf::Keyboard::isKeyPressed() for NON MacOs users
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::W)) {
        moveDirection.y -= 1.0f;
    }

    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::S)) {
        moveDirection.y += 1.0f;
    }

    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::A)) {
        moveDirection.x -= 1.0f;
        m_playerShip->rotate(-(degreesPerSecond) * deltaTime);
    }

    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::D)) {
        moveDirection.x += 1.0f;
        m_playerShip->rotate(+(degreesPerSecond) * deltaTime);
    }

    moveDirection = gbh::math::normalize(moveDirection);
    m_playerShip->getPhysicsBody()->applyForceToCenter(moveDirection*accelerationForce);
}

void MainGameScene::onKeyboardEvent(sf::Event& event)
{
    if (event.key.code == sf::Keyboard::O)
    {
        setDrawPhysicsDebug(true);
    }
    
    if (event.key.code == sf::Keyboard::X)
    {
        setDrawPhysicsDebug(false);
    }
}
