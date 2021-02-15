#include "titlescene.h"

#include <iostream>
#include <random>        // mt19937, uniform_int_distribution
#include <ctime>        // time
#include <cmath>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"

const std::string kTitleMusic = "../assets/music/titlescreen.ogg";
const std::string kTitleScreenFont = "../assets/fonts/roboto-regular.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kMediumAsteroid01 = "../assets/gfx/asteroid-medium-01.png";

// Set up our random generator as a global object we can use anywhere
// Seed it with the current time so it's different every time
std::mt19937 randomGenerator = std::mt19937(std::time(nullptr));



void TitleScene::onInitializeScene()
{
	m_robotoFont.loadFromFile(kTitleScreenFont);
	m_titleMusic.openFromFile(kTitleMusic);
	m_titleMusic.setVolume(25);
    
    createPhysicsWorld(sf::Vector2f());
    setDrawPhysicsDebug(true);
    setDebugPhysicsEvents(true);
    
    createBackground();
    createAsteroids();
    createPlayerShip();
    createUI();
}


void TitleScene::onUpdate(double deltaTime)
{
    m_sceneTimer += deltaTime;
    m_titleNode->setPosition(640, 100.0f + (40.0f * sin(m_sceneTimer)));
        
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::Space))
    {
        m_ship->getPhysicsBody()->applyForceToCenter(sf::Vector2f(0, 5000.0f));
    }
}


void TitleScene::onMouseEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		std::shared_ptr<gbh::Node> node = this->getNodeAtViewPoint((float)event.mouseButton.x, (float)event.mouseButton.y);

		if (node)
		{
			std::cout << "Clicked On: " << node->getName() << "\n";
            
            if (node->getName() == "StartButton")
            {
                gbh::Game::getInstance().changeScene("setup");
            }
		}
        else
        {
            m_ship->getPhysicsBody()->applyAngularImpulse(100.0f);
            m_ship->getPhysicsBody()->applyImpulseToCenter(sf::Vector2f(100.0f, -1000.0f));
        }
    }
}


void TitleScene::onKeyboardEvent(sf::Event &event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::U)
        {
            setDrawPhysicsDebug(!getDrawPhysicsDebug());
        }
        
        if (event.key.code == sf::Keyboard::Space)
        {
            gbh::Game::getInstance().changeScene("game");
        }
    }
}


void TitleScene::onBeginPhysicsContact(const gbh::PhysicsContact& contact)
{

}


void TitleScene::onShowScene()
{
	m_titleMusic.play();
}


void TitleScene::onHideScene()
{
	m_titleMusic.stop();
}


void TitleScene::createBackground()
{
    std::shared_ptr<gbh::SpriteNode> bgNode = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    bgNode->setName("Background");
    bgNode->setPosition(640, 360);
    addChild(bgNode);
    
    std::shared_ptr<gbh::Node> worldBoundary = std::make_shared<gbh::Node>();
    worldBoundary->setName("WorldBoundary");
    worldBoundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(1240, 700)));
    worldBoundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
    worldBoundary->setPosition(640, 360);
    addChild(worldBoundary);
}


void TitleScene::createAsteroids()
{
    if (m_asteroids.size() > 0)
    {
        std::cout << "[TitleScene] Asteroids already exist! \n";
        return;
    }
    
    std::uniform_int_distribution<int> xRandom(40, 1240);
    std::uniform_int_distribution<int> yRandom(40, 680);
    std::uniform_real_distribution<float> randomVelocity(-40.0f, 40.0f);

    for(int i = 0; i < 6; ++i)
    {
        const gbh::PhysicsMaterial asteroidMat = gbh::PhysicsMaterial(1.0f, 1.0f, 0.0f);
    
        float x = xRandom(randomGenerator);
        float y = yRandom(randomGenerator);
        sf::Vector2f velocity = sf::Vector2f(randomVelocity(randomGenerator), randomVelocity(randomGenerator));

        // Avoid middle 100x100 box where the player ship is
        while (abs((1280/2)-x) < 50 && abs((720/2)-y) < 50) {
            x = xRandom(randomGenerator);
            y = yRandom(randomGenerator);
        }
                
        std::shared_ptr<gbh::SpriteNode> asteroid = std::make_shared<gbh::SpriteNode>(kMediumAsteroid01);
        asteroid->setPhysicsBody(getPhysicsWorld()->createCircle(45, asteroidMat));
        asteroid->getPhysicsBody()->setAngularVelocity(randomVelocity(randomGenerator));
        asteroid->setPosition(x, y);
        asteroid->getPhysicsBody()->setLinearVelocity(velocity);
        
        m_asteroids.push_back(asteroid);
        addChild(asteroid);
    }
}


void TitleScene::createPlayerShip()
{
    m_ship = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_ship->setName("PlayerShip");
    m_ship->setPosition(640, 360);
    m_ship->setOrigin(0.5f, 0.5f);
    m_ship->setPhysicsBody(getPhysicsWorld()->createBox(sf::Vector2f(80.f, 120.f)));
    m_ship->getPhysicsBody()->setType(gbh::PhysicsBodyType::Dynamic);
    m_ship->getPhysicsBody()->setAngularVelocity(20.0f);
    
    m_ship->setBeginContactCallback([this] (const gbh::PhysicsContact& contact) {
        std::cout << "Collided With: " << contact.otherNode(m_ship.get())->getName() << "\n";
        m_ship->setColor(sf::Color::Yellow);
    });
    
    addChild(m_ship);
}


void TitleScene::createUI()
{
    m_titleNode = std::make_shared<gbh::TextNode>("Space Race", m_robotoFont, 60);
    m_titleNode->setPosition(640, 100);
    m_titleNode->setName("Title");
    addChild(m_titleNode);

    std::shared_ptr<gbh::ShapeNode> startButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(1280, 60)));
    startButton->setPosition(640, 560);
    startButton->getShape()->setFillColor(sf::Color(0, 255, 0, 64));
    startButton->setName("StartButton");
    addChild(startButton);

    std::shared_ptr<gbh::TextNode> startNode = std::make_shared<gbh::TextNode>("Start Game", m_robotoFont);
    startNode->setPosition(640, 60/2);
    startNode->setName("StartButton");
    startButton->addChild(startNode);
}
