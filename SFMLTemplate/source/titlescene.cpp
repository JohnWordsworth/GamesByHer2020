#include "titlescene.h"

#include <iostream>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"

const std::string kTitleMusic = "../assets/music/titlescreen.ogg";
const std::string kTitleScreenFont = "../assets/fonts/roboto-regular.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";


void TitleScene::onInitializeScene()
{
	m_robotoFont.loadFromFile(kTitleScreenFont);
	m_titleMusic.openFromFile(kTitleMusic);
	m_titleMusic.setVolume(25);
    
    createPhysicsWorld(sf::Vector2f(0.0f, -9.81f));
    setDrawPhysicsDebug(true);
    setDebugPhysicsEvents(true);
    
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

	std::shared_ptr<gbh::TextNode> textNode = std::make_shared<gbh::TextNode>("Space Race", m_robotoFont, 60);
	textNode->setPosition(640, 30);
	textNode->setName("Title");
	addChild(textNode);

	m_ship = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_ship->setName("PlayerShip");
    m_ship->setPosition(640, 360);
    m_ship->setOrigin(0.5f, 0.5f);
    m_ship->setPhysicsBody(getPhysicsWorld()->createBox(sf::Vector2f(80.f, 120.f)));
    m_ship->getPhysicsBody()->setType(gbh::PhysicsBodyType::Dynamic);
    
    m_ship->setBeginContactCallback([this] (const gbh::PhysicsContact& contact) {
        std::cout << "Collided With: " << contact.otherNode(m_ship.get())->getName() << "\n";
        m_ship->setColor(sf::Color::Yellow);
    });
    
	addChild(m_ship);

	std::shared_ptr<gbh::ShapeNode> startButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(1280, 60)));
	startButton->setPosition(640, 560);
    startButton->setOrigin(0.5f, 0.5f);
	startButton->getShape()->setFillColor(sf::Color(0, 255, 0, 64));
	startButton->setName("StartButton");
	addChild(startButton);

	std::shared_ptr<gbh::TextNode> startNode = std::make_shared<gbh::TextNode>("Start Game", m_robotoFont);
	startNode->setPosition(640, 10);
    startNode->setOrigin(0.5f, 0.0f);
	startNode->setName("StartButton");
	startButton->addChild(startNode);
    
    m_timer = std::make_shared<gbh::TextNode>("0.0", m_robotoFont, 14);
    m_timer->setOrigin(1.0f, 1.0f);
    m_timer->setPosition(1260, 700);
    addChild(m_timer);
}


void TitleScene::onUpdate(double deltaTime)
{
    std::shared_ptr<gbh::Node> titleNode = getFirstChildWithName("Title", false);
    
    // Drift title node down the screen - just for fun
    if (titleNode)
    {
        if (titleNode->getPosition().y < 300.0f)
        {
            titleNode->move(0, 50.f * (float)deltaTime);
        }
        
        const float degreesPerSecond = 45.0f;
        titleNode->rotate((float)deltaTime * degreesPerSecond);
    }
    
    if (gbh::Game::getInstance().isKeyPressed(sf::Keyboard::Space))
    {
        m_ship->getPhysicsBody()->applyForceToCenter(sf::Vector2f(0, 5000.0f));
    }
    
    m_timerValue += deltaTime;
    m_timer->setString(std::to_string(m_timerValue));
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
                gbh::Game::getInstance().changeScene("game");
            }
		}
    
        m_ship->getPhysicsBody()->applyAngularImpulse(100.0f);
        m_ship->getPhysicsBody()->applyImpulseToCenter(sf::Vector2f(100.0f, -1000.0f));
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

