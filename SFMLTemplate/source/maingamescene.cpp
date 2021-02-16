#include "maingamescene.h"
#include "gamestate.h"

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"
#include "sfml-engine/mathutils.h"
#include "sfml-engine/texturecache.h"

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

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
    
    m_timer = std::make_shared<gbh::TextNode>("0.00", m_robotoFont, 24);
    m_timer->setOrigin(1.0f, 1.0f);
    m_timer->setPosition(1270, 700);
    getOverlay().addChild(m_timer);
}


void MainGameScene::onShowScene()
{
    loadLevel(GameState::getInstance().selectedLevel);
    advanceCheckpoints();
    
    m_gameMusic.play();
    m_gameFinished = false;
}


void MainGameScene::onHideScene()
{
    // Clear the world!
    removeAllChildren(true);
    m_ship = nullptr;
    m_camera = nullptr;
    m_checkPoints.clear();
    
    m_gameMusic.stop();
    
    if (m_gameOverText)
    {
        m_gameOverText->removeFromParent(true);
        m_gameOverText = nullptr;
    }
}


void MainGameScene::onUpdate(double deltaTime)
{
    if (m_gameFinished)
    {
        return;
    }
    
    m_timerValue += deltaTime;
    
    char buffer[32];
    sprintf(buffer, "%.2f", m_timerValue);
    m_timer->setString(buffer);
    
    advancedMovementUpdate(deltaTime);
}


void MainGameScene::loadLevel(const std::string &filename)
{
    
    std::ifstream file(filename);
    nlohmann::json jsonFile;
    
    try
    {
        jsonFile = nlohmann::json::parse(file);
    }
    catch(const std::exception& ex)
    {
        std::cout << "Failed to load level from file: " << filename << ": " << ex.what() << "\n";
        return;
    }
    
    nlohmann::json spawnPoint = jsonFile["spawnPoint"];
    sf::Vector2f shipPosition = sf::Vector2f(640, 360);
    
    if (spawnPoint.is_object())
    {
        if (spawnPoint["x"].is_number()) { shipPosition.x = spawnPoint["x"].get<float>(); }
        if (spawnPoint["y"].is_number()) { shipPosition.y = spawnPoint["y"].get<float>(); }
    }
    
    std::shared_ptr<gbh::SpriteNode> bgNode = std::make_shared<gbh::SpriteNode>(kStarfield);
    bgNode->setName("background");
    bgNode->setPosition(640, 360);
    addChild(bgNode);
    
    std::shared_ptr<gbh::Node> worldBoundary = std::make_shared<gbh::Node>();
    worldBoundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(2480, 1400)));
    worldBoundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
    worldBoundary->setPosition(0, 0);
    addChild(worldBoundary);

    createPlayerShip();
    m_ship->setPosition(shipPosition);
    
    m_camera = std::make_shared<FollowCameraNode>();
    m_camera->setPosition(shipPosition);
    m_camera->setTarget(m_ship);
    addChild(m_camera);
    setCamera(m_camera);
    
    nlohmann::json checkpoints = jsonFile["checkpoints"];
    
    if (checkpoints.is_array())
    {
        for(int i = 0; i < checkpoints.size(); ++i)
        {
            float x = checkpoints[i]["x"].get<float>();
            float y = checkpoints[i]["y"].get<float>();

            std::shared_ptr<gbh::SpriteNode> node = std::make_shared<gbh::SpriteNode>(kCheckpoint);
            node->setColor(kInactiveCheckpoint);
            node->setPhysicsBody(getPhysicsWorld()->createCircle(50));
            node->getPhysicsBody()->makeSensor();
            node->getPhysicsBody()->setEnabled(false);
            node->setPosition(x, y);
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
        
        m_currentCheckpoint = -1;
    }
    
    nlohmann::json objects = jsonFile["objects"];
    nlohmann::json actors = jsonFile["actors"];
    
    if (objects.is_object() && actors.is_array())
    {
        for(int i = 0; i < actors.size(); ++i)
        {
            nlohmann::json actor = actors[i];
            nlohmann::json object = objects[actor["object"].get<std::string>()];
            
            if (object.is_object())
            {
                auto node = std::make_shared<gbh::SpriteNode>(object["image"].get<std::string>());
                node->setPosition(actor["x"].get<float>(), actor["y"].get<float>());
                
                if (object["body"].is_object())
                {
                    if (object["body"]["radius"].is_number())
                    {
                        node->setPhysicsBody(getPhysicsWorld()->createCircle(object["body"]["radius"].get<float>()));
                    }
                }
                
                addChild(node);
            }
        }
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
        if (event.key.code == sf::Keyboard::Space && m_gameFinished)
        {
            gbh::Game::getInstance().changeScene("title");
        }
        
        if (event.key.code == sf::Keyboard::Escape)
        {
            gbh::Game::getInstance().changeScene("title");
        }
        
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


void MainGameScene::onBeginPhysicsContact(const gbh::PhysicsContact& contact)
{

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
        m_gameOverText = std::make_shared<gbh::TextNode>("Course Finished! Press Space to Continue", m_robotoFont, 40);
        m_gameOverText->setPosition(640, 360);
        getOverlay().addChild(m_gameOverText);
        
        m_gameFinished = true;
    }
}


void MainGameScene::createPlayerShip()
{
    // Need to make the ship more dense now that it consists of less mass / smaller shape!
    gbh::PhysicsMaterial shipMaterial;
    shipMaterial.density = 3.0f;
    
    m_ship = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_ship->setOrigin(0.5f, 0.5f);
    m_ship->setScale(0.5f, 0.5f);
    m_ship->setPhysicsBody(getPhysicsWorld()->createEmptyBody());
    m_ship->getPhysicsBody()->addBox(sf::Vector2f(10, 60), sf::Vector2f(), shipMaterial);
    m_ship->getPhysicsBody()->addBox(sf::Vector2f(40, 10), sf::Vector2f(0, 5), shipMaterial);
    m_ship->getPhysicsBody()->setType(gbh::PhysicsBodyType::Dynamic);
    m_ship->getPhysicsBody()->setLinearDamping(2.0f);
    m_ship->getPhysicsBody()->setFixedRotation(true);
    addChild(m_ship);
}
