#pragma once
#include "sfml-engine/scene.h"
#include "cameranode.h"
#include "levelselectscene.h"

class MainGameScene : public gbh::Scene
{
public:
    void endGameScene();
    
    LevelSelectScene levelSelected;

protected:
    void onInitializeScene() override;
    virtual void onKeyboardEvent(sf::Event& event) override;

    virtual void onUpdate(double deltaTime) override;
    virtual void onBeginPhysicsContact(const gbh::PhysicsContact& contact) override;
    
    void onShowScene() override;

    void loadLevel(const std::string& fileName);
    
private:
    void advancedCheckPoints();

    std::shared_ptr<gbh::SpriteNode> m_playerShip;
    std::shared_ptr<FollowCameraNode> m_followCamera;
    std::shared_ptr<gbh::TextNode> m_timerText;

    std::vector<std::shared_ptr<gbh::SpriteNode>> m_checkPoints;
    int m_currentCheckPoint = -1;
    double m_playerTime;
    bool m_courseFinished = false;
    
    std::shared_ptr<gbh::SpriteNode> m_asteroidObstacle01;
    sf::Font m_orbitronFont;
};
