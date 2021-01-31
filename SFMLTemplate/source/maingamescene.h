#pragma once
#include "sfml-engine/scene.h"

class MainGameScene : public gbh::Scene {

protected:
    void onInitializeScene() override;

    virtual void onUpdate(double deltaTime) override;
    
private:
    std::shared_ptr<gbh::SpriteNode> m_playerShip;
};
