#include "credits.h"
#include "sfml-engine/game.h"

const std::string kTitleScreenFont = "../assets/fonts/orbitron.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";

void CreditScene::onInitializeScene() {
    m_orbitronFont.loadFromFile(kTitleScreenFont);

    std::shared_ptr<gbh::SpriteNode> spriteCreditsBg = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    spriteCreditsBg->setName("creditsBg");
    addChild(spriteCreditsBg);
    
    std::shared_ptr<gbh::TextNode> creditsTxt = std::make_shared<gbh::TextNode>("CREDITS", m_orbitronFont, 60);
    creditsTxt->setPosition(650, 100);
    creditsTxt->setName("creditsTitle");
    addChild(creditsTxt);
    
    std::shared_ptr<gbh::ShapeNode> returnBtn = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(200, 60)));
    returnBtn->setPosition(660, 590);
    returnBtn->getShape()->setFillColor(sf::Color(0, 255, 0, 64));
    returnBtn->getShape()->setOutlineThickness(10.f);
    returnBtn->setName("ReturnButton");
    addChild(returnBtn);
    
    std::shared_ptr<gbh::TextNode> returnTxt = std::make_shared<gbh::TextNode>("BACK", m_orbitronFont);
    returnTxt->setPosition(95, 20);
    returnTxt->setName("ReturnButton");
    returnBtn->addChild(returnTxt);
}

void CreditScene::onMouseEvent(sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2 mousePosition = { (float)event.mouseButton.x, (float)event.mouseButton.y };
        std::shared_ptr<gbh::Node> node = getNodeAtViewPoint(mousePosition);
        
        if (node->getName() == "ReturnButton")
        {
            gbh::Game::getInstance().changeScene("title");
        }
    }
}
