#pragma once 

#include "followcameranode.h"

#include "sfml-engine/scene.h"

#include <SFML/Audio.hpp>


class MainGameScene : public gbh::Scene
{
protected:
	void onInitializeScene() override;
    void onShowScene() override;
    void onHideScene() override;

	void onUpdate(double deltaTime) override;
	void onMouseEvent(sf::Event& event) override;
    void onKeyboardEvent(sf::Event& event) override;
    void onJoystickEvent(sf::Event& event) override;
    void onBeginPhysicsContact(const gbh::PhysicsContact& contact) override;
    
    void loadLevel(const std::string& filename);

    void simpleMovementUpdate(double deltaTime);
    void advancedMovementUpdate(double deltaTime);
    
    void advanceCheckpoints();

private:
	sf::Font m_robotoFont;
	sf::Music m_gameMusic;
    
    std::shared_ptr<gbh::SpriteNode> m_ship;
    std::shared_ptr<FollowCameraNode> m_camera;
    
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_checkPoints;
    int m_currentCheckpoint = -1;
    
    std::shared_ptr<gbh::TextNode> m_timer;
    double m_timerValue = 0.0;
    
    std::shared_ptr<gbh::TextNode> m_gameOverText;
    bool m_gameFinished = false;
};
