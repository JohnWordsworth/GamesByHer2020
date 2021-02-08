#pragma once
#include "sfml-engine/scene.h"
#include "cameranode.h"

class MainGameScene : public gbh::Scene
{
protected:
    void onInitializeScene() override;
    virtual void onKeyboardEvent(sf::Event& event) override;

    virtual void onUpdate(double deltaTime) override;
    
private:
    std::shared_ptr<gbh::SpriteNode> m_playerShip;
    std::shared_ptr<FollowCameraNode> m_followCamera;
    
    std::shared_ptr<gbh::SpriteNode> m_asteroidObstacle01;
};
