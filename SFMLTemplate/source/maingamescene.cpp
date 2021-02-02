#include "maingamescene.h"

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"
#include "sfml-engine/mathutils.h"
#include "sfml-engine/texturecache.h"

#include <iostream>


const std::string kGameMusic = "../assets/music/maingame.ogg";
const std::string kGameFont = "../assets/fonts/roboto-regular.ttf";
const std::string kStarfield = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kCheckpoint = "../assets/gfx/checkpoint.png";

static const sf::Color kInactiveCheckpoint = sf::Color(255, 255, 255, 64);
static const sf::Color kNextCheckpoint = sf::Color(64, 64, 255, 192);
static const sf::Color kDoneCheckpoint = sf::Color(64, 255, 64, 128);


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
    
    sf::Texture* bgTexture = gbh::TextureCache::getInstance().getTextureAtPath(kStarfield);
    
    if (bgTexture) {
        bgTexture->setSmooth(true);
        bgTexture->setRepeated(true);
    }
    
    std::shared_ptr<gbh::Node> worldBoundary = std::make_shared<gbh::Node>();
    worldBoundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(2480, 1400)));
    worldBoundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
    worldBoundary->setPosition(0, 0);
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
    
    m_camera = std::make_shared<FollowCameraNode>();
    m_camera->setPosition(640, 360);
    m_camera->setTarget(m_ship);
    addChild(m_camera);
    setCamera(m_camera);
    
    std::vector<sf::Vector2f> checkpoints = {
        sf::Vector2f(520, 320),
        sf::Vector2f(100, 20),
        sf::Vector2f(-300, -240),
        sf::Vector2f(0, -320),
        sf::Vector2f(-540, -320),
        sf::Vector2f(-660, 0),
        sf::Vector2f(-300, 440),
    };
    
    for(int i = 0; i < checkpoints.size(); ++i)
    {
        std::shared_ptr<gbh::SpriteNode> node = std::make_shared<gbh::SpriteNode>(kCheckpoint);
        node->setColor(kInactiveCheckpoint);
        node->setPhysicsBody(getPhysicsWorld()->createCircle(50));
        node->getPhysicsBody()->makeSensor();
        node->getPhysicsBody()->setEnabled(false);
        node->setPosition(checkpoints[i]);
        node->setName("checkpoint");
        
        node->setBeginContactCallback([this](const gbh::PhysicsContact& contact) {
            if (contact.containsNode(m_ship.get()))
            {
                advanceCheckpoints();
            }
        });
                
        m_checkPoints.push_back(node);
        addChild(node);
    }
    
    m_timer = std::make_shared<gbh::TextNode>("0.00", m_robotoFont, 24);
    m_timer->setOrigin(1.0f, 1.0f);
    m_timer->setPosition(1270, 700);
    getOverlay().addChild(m_timer);
    
    advanceCheckpoints();
}


void MainGameScene::onUpdate(double deltaTime)
{
    advancedMovementUpdate(deltaTime);
    
    static const float cameraSpeed = 200.0f;
    static const float rotateSpeed = 90.0f;
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::Left)) {
        m_camera->move(-cameraSpeed * (float)deltaTime, 0.0f);
    }
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::Right)) {
        m_camera->move( cameraSpeed * (float)deltaTime, 0.0f);
    }
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::Up)) {
        m_camera->move(0.0f, -cameraSpeed * (float)deltaTime);
    }
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::Down)) {
        m_camera->move(0.0f,  cameraSpeed * (float)deltaTime);
    }
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
    const float acceleration = 2000.0f;

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
        sf::Vector2f moveDirection = m_ship->forwardVector();
        m_ship->getPhysicsBody()->applyForceToCenter(moveDirection * acceleration);
    }
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::S))
    {
        sf::Vector2f moveDirection = m_ship->forwardVector();
        m_ship->getPhysicsBody()->applyForceToCenter(-moveDirection * acceleration * 0.5f);
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
        
        if (event.key.code == sf::Keyboard::J)
        {
            m_camera->setScale(0.5f, 0.5f);
        }
        
        if (event.key.code == sf::Keyboard::K)
        {
            m_camera->setScale(1.0f, 1.0f);
        }

        if (event.key.code == sf::Keyboard::L)
        {
            m_camera->setScale(2.0f, 2.0f);
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


void MainGameScene::advanceCheckpoints()
{
    
    if (m_currentCheckpoint >= 0 && m_currentCheckpoint < m_checkPoints.size())
    {
        m_checkPoints[m_currentCheckpoint]->setColor(kDoneCheckpoint);
        m_checkPoints[m_currentCheckpoint]->getPhysicsBody()->setEnabled(false);
        m_currentCheckpoint++;
    }
    else
    {
        m_currentCheckpoint = 0;
    }
    
    if (m_currentCheckpoint < m_checkPoints.size())
    {
        m_checkPoints[m_currentCheckpoint]->setColor(kNextCheckpoint);
        m_checkPoints[m_currentCheckpoint]->getPhysicsBody()->setEnabled(true);
    }
    else
    {
        std::cout << "Completed Course! \n";
    }
}
