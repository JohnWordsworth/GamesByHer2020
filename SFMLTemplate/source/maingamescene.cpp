#include "maingamescene.h"
#include "gamestate.h"

#include "sfml-engine/game.h"
#include "sfml-engine/mathutils.h"
#include "sfml-engine/textnode.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kAsteroid01 = "../assets/gfx/asteroid-small-01.png";
const std::string kCheckpoint = "../assets/gfx/checkpoint.png";
const std::string kTitleScreenFont = "../assets/fonts/orbitron.ttf";

//Ship size
const sf::Vector2f shipSize = sf::Vector2f(80.0f, 120.0f);

//Checkpoint colors
static const sf::Color kInactiveCheckpoint = sf::Color(255, 255, 255, 64);
static const sf::Color kNextCheckpoint = sf::Color(64, 64, 255, 192);
static const sf::Color kDoneCheckpoint = sf::Color(64, 255, 64, 128);

MainGameScene& MainGameScene::InstanceOf() {
    static MainGameScene mainGame;
    return mainGame;
}

void MainGameScene::onInitializeScene() {
    //load font
    m_orbitronFont.loadFromFile(kTitleScreenFont);

    //Add initial physics and player ship
    createPhysicsWorld(sf::Vector2f(0.0f,0.0f));
    
    //add timer
    m_timerText = std::make_shared<gbh::TextNode>("0", m_orbitronFont, 24);
    m_timerText->setOrigin(1.0f, 1.0f);
    m_timerText->setPosition(1270, 700);
    getOverlay().addChild(m_timerText);
}

void MainGameScene::onShowScene()
{
    loadLevel(GameState::getInstance().selectedLevel);
    advancedCheckPoints();
}

void MainGameScene::addWorldBoundary()
{
    // Add a boundary that is almost as big as the screen (1270, 710) and centered.
    std::shared_ptr<gbh::Node> boundary = std::make_shared<gbh::Node>();
    boundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(2048, 2048)));
    boundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
    boundary->setPosition(1280.0f/2.0f, 720.0f/2.0f);
    addChild(boundary);
}

void MainGameScene::addPlayerShip(const int spawnPointX, const int spawnPointY)
{
    m_playerShip = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_playerShip->setName("playership");
    m_playerShip->setPosition(spawnPointX, spawnPointY);
    m_playerShip->setScale(0.5f, 0.5f);
    m_playerShip->setPhysicsBody(getPhysicsWorld()->createBox(shipSize * 0.5f));
    m_playerShip->getPhysicsBody()->setLinearDamping(2.0f);
    m_playerShip->getPhysicsBody()->setFixedRotation(true);
    addChild(m_playerShip);
    
    //Initialize camera & setPosition to match current position of playerShip
    m_followCamera = std::make_shared<FollowCameraNode>();
    m_followCamera->setTarget(m_playerShip);
    m_followCamera->setPosition(spawnPointX, spawnPointY);

    addChild(m_followCamera);
    setCamera(m_followCamera);
}

void MainGameScene::loadLevel(const std::string &filename)
{
    addWorldBoundary();

    //load from files
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
    
    //Initialize background
    nlohmann::json bg = jsonFile["config"]["background"];

    if (bg.is_string())
    {
        std::shared_ptr<gbh::SpriteNode> spriteMainBg = std::make_shared<gbh::SpriteNode>(bg);
        spriteMainBg->setPosition(640, 360);
        spriteMainBg->setName("GameBackground");
        addChild(spriteMainBg);
    }
    
    //Initialize checkpoints
    nlohmann::json checkPoints = jsonFile["checkpoints"];
    
    if (checkPoints.is_array())
    {
        for(int i = 0; i < checkPoints.size(); ++i)
        {
            float x = checkPoints[i]["x"].get<float>();
            float y = checkPoints[i]["y"].get<float>();

            std::shared_ptr<gbh::SpriteNode> node = std::make_shared<gbh::SpriteNode>(kCheckpoint);
            node->setColor(kInactiveCheckpoint);
            node->setPhysicsBody(getPhysicsWorld()->createCircle(50));
            node->getPhysicsBody()->makeSensor();
            node->getPhysicsBody()->setEnabled(false);
            node->setPosition(x, y);
            node->setName("checkpoint");

            m_checkPoints.push_back(node);
            addChild(node);
        }
        
        m_currentCheckPoint = -1;
    }
    
    //Create ship
    nlohmann::json spawnPoint = jsonFile["config"]["spawnPoint"];
    
    if (spawnPoint.is_object())
    {
        if (spawnPoint["x"].is_number() && spawnPoint["y"].is_number())
        {
            addPlayerShip(spawnPoint["x"].get<float>(), spawnPoint["y"].get<float>());
        }
    }
    else
    {
        addPlayerShip(640, 360);
    }
    
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
        m_courseFinished = true;
        std::cout << "Completed Course! \n";
        endGameScene();
    }
}

void MainGameScene::onUpdate(double deltaTime)
{
    //add time to timer
    m_playerTime += deltaTime;

    if (m_courseFinished == false)
    {
        float currentTime = floor(m_playerTime * 10) / 10;
        m_timerText->setString(std::to_string(currentTime));
    }

    //player movement
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
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Space)
        {
            if (m_courseFinished)
            {
                gbh::Game::getInstance().changeScene("title");
            }
        }
        
        if (event.key.code == sf::Keyboard::O)
        {
            setDrawPhysicsDebug(true);
        }
        
        if (event.key.code == sf::Keyboard::X)
        {
            setDrawPhysicsDebug(false);
        }
    }
}

void MainGameScene::endGameScene() {
    std::shared_ptr<gbh::ShapeNode> endScene = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(1270, 710)));
    endScene->setPosition(640, 360);
    endScene->getShape()->setFillColor(sf::Color(0, 0, 0, 128));
    getOverlay().addChild(endScene);
    
    std::shared_ptr<gbh::TextNode> endSceneTxt = std::make_shared<gbh::TextNode>("YOU COMPLETED THE COURSE", m_orbitronFont, 60);
    endSceneTxt->setPosition(650, 300);
    getOverlay().addChild(endSceneTxt);
    
    std::shared_ptr<gbh::TextNode> endSceneTxtPrompt = std::make_shared<gbh::TextNode>("Press spacebar to continue", m_orbitronFont, 60);
    endSceneTxtPrompt->setPosition(650, 450);
    getOverlay().addChild(endSceneTxtPrompt);
}
