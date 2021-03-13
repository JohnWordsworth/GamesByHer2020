#pragma once
#include "sfml-engine/scene.h"
#include "cameranode.h"
#include "levelselectscene.h"

class MainGameScene : public gbh::Scene
{
public:
    void endGameScene();
    static MainGameScene& InstanceOf();

protected:
    void onInitializeScene() override;
    virtual void onKeyboardEvent(sf::Event& event) override;

    virtual void onUpdate(double deltaTime) override;
    virtual void onBeginPhysicsContact(const gbh::PhysicsContact& contact) override;
    
    void onShowScene() override;
    void onHideScene() override;

    void loadLevel(const std::string& fileName);
    
private:
    void addBasicGraphics(const std::string backgroundImg, const float boundaryX, const float boundaryY);
    void addPlayerShip(const float spawnPointX, const float spawnPointY);
    void addWorldBoundary(const float positionX, const float positionY);
    void advancedCheckPoints();

    std::shared_ptr<gbh::SpriteNode> m_playerShip;
    std::shared_ptr<FollowCameraNode> m_followCamera;
    std::shared_ptr<gbh::TextNode> m_timerText;
    std::shared_ptr<gbh::TextNode> m_gameOverTxt;

    std::vector<std::shared_ptr<gbh::SpriteNode>> m_checkPoints;
    int m_currentCheckPoint = -1;
    double m_playerTime;
    bool m_courseFinished = false;
    
    std::shared_ptr<gbh::SpriteNode> m_asteroidObstacle01;
    
    sf::Font m_orbitronFont;
};
