#pragma once

#include "sfml-engine/scene.h"

class MainGameScene : public gbh::Scene {
protected:
    void onInitializeScene() override;
    
private:
    std::shared_ptr<gbh::SpriteNode> m_playerShip;
};
