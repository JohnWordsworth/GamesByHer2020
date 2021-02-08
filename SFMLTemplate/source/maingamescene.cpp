#include "maingamescene.h"
#include "sfml-engine/game.h"
#include "sfml-engine/mathutils.h"
#include <iostream>

const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kAsteroid01 = "../assets/gfx/asteroid-small-01.png";
const std::string kCheckpoint = "../assets/gfx/checkpoint.png";

//Checkpoint colors
static const sf::Color kInactiveCheckpoint = sf::Color(255, 255, 255, 64);
static const sf::Color kNextCheckpoint = sf::Color(64, 64, 255, 192);
static const sf::Color kDoneCheckpoint = sf::Color(64, 255, 64, 128);

void MainGameScene::onInitializeScene() {
    std::shared_ptr<gbh::SpriteNode> spriteMainBg = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    spriteMainBg->setPosition(640, 360);
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
    
    //add dynamic asteroid
    m_asteroidObstacle01 = std::make_shared<gbh::SpriteNode>(kAsteroid01);
    m_asteroidObstacle01->setOrigin(0.5f, 0.5f);
    m_asteroidObstacle01->setPosition(830, 150);
    m_asteroidObstacle01->setName("Asteroid2");
    
    std::shared_ptr<gbh::PhysicsBody> body = getPhysicsWorld()->createBox(sf::Vector2f(20.f, 20.0f));
    body->setType(gbh::PhysicsBodyType::Dynamic);
    body->setLinearDamping(20.0f);

    m_asteroidObstacle01->setPhysicsBody(body);
    m_asteroidObstacle01->getPhysicsBody()->setAngularDamping(0);
    m_asteroidObstacle01->getPhysicsBody()->applyTorque(20.0f, true);
    addChild(m_asteroidObstacle01);
    
    //initialize camera
    m_followCamera = std::make_shared<FollowCameraNode>();
    m_followCamera->setTarget(m_playerShip);
    m_followCamera->setPosition(640, 360);

    addChild(m_followCamera);
    setCamera(m_followCamera);
    
    //initialize checkpoints

    std::vector<sf::Vector2f> checkPointPositions = {
        sf::Vector2f(640.0f, 720.0f),
        sf::Vector2f(1240.0f, 200.0f),
        sf::Vector2f(80.0f, 400.0f),
    };

    for(int i = 0; i < checkPointPositions.size(); ++i)
    {
        sf::Vector2f position = checkPointPositions[i];

        std::shared_ptr<gbh::SpriteNode> node = std::make_shared<gbh::SpriteNode>(kCheckpoint);
        node->setColor(kInactiveCheckpoint);
        node->setPhysicsBody(getPhysicsWorld()->createCircle(50));
        node->getPhysicsBody()->makeSensor();
        node->getPhysicsBody()->setEnabled(false);
        node->setPosition(checkPointPositions[i]);
        node->setName("checkpoint");

        m_checkPoints.push_back(node);
        addChild(node);
    }
    
    //Call function to initialize
    advancedCheckPoints();
    
}

void MainGameScene::onBeginPhysicsContact(const gbh::PhysicsContact& contact)
{
    if (contact.containsNode(m_playerShip.get()))
    {
        gbh::Node* otherNode = contact.otherNode(m_playerShip.get());

        if (otherNode && otherNode->getName() == "checkpoint")
        {
            advancedCheckPoints();
        }
    }
}

void MainGameScene::advancedCheckPoints()
{
    if (m_currentCheckPoint >= 0 && m_currentCheckPoint < m_checkPoints.size())
    {
        m_checkPoints[m_currentCheckPoint]->setColor(kDoneCheckpoint);
        m_checkPoints[m_currentCheckPoint]->getPhysicsBody()->setEnabled(false);
        m_currentCheckPoint++;
    }
    else
    {
        m_currentCheckPoint = 0;
    }
    
    if (m_currentCheckPoint < m_checkPoints.size())
    {
        m_checkPoints[m_currentCheckPoint]->setColor(kNextCheckpoint);
        m_checkPoints[m_currentCheckPoint]->getPhysicsBody()->setEnabled(true);
    }
    else
    {
        std::cout << "Completed Course! \n";
    }
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
