#include "maingamescene.h"

const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";

void MainGameScene::onInitializeScene() {
    std::shared_ptr<gbh::SpriteNode> spriteMainBg = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
    spriteMainBg->setName("GameBackground");
    addChild(spriteMainBg);
    
    m_playerShip = std::make_shared<gbh::SpriteNode>(kPlayerShip);
    m_playerShip->setName("playership");
    m_playerShip->setOrigin(0.5f, 0.5f);
    m_playerShip->setPosition(620, 300);
    addChild(m_playerShip);
}
