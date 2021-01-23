#include "maingamescene.h"
#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"

#include <iostream>

const std::string kStarfield = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";

void MainGameScene::onInitializeScene() {
    std::cout << "Hello from onInitialize in MainGameScene! \n";
    
    //add space background
    std::shared_ptr<gbh::SpriteNode> background = std::make_shared<gbh::SpriteNode>(kStarfield);
    background->setName("background");
    addChild(background);
    std::cout << "Added background. \n";
    
    //add player ship
    m_playerShip = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_playerShip->setName("playership");
    m_playerShip->setOrigin(0.5f, 0.5f);
    m_playerShip->setPosition(640, 360);
    addChild(m_playerShip);
    std::cout << "Added ship. \n";
    
}
