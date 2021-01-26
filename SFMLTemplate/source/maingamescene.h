#pragma once 

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>


class MainGameScene : public gbh::Scene
{
protected:
	void onInitializeScene() override;

	void onUpdate(double deltaTime) override;
	void onMouseEvent(sf::Event& event) override;
    void onKeyboardEvent(sf::Event& event) override;
    void onJoystickEvent(sf::Event& event) override;
    
    void simpleMovementUpdate(double deltaTime);
    void advancedMovementUpdate(double deltaTime);
    
	void onShowScene() override;
	void onHideScene() override;

private:
	sf::Font m_robotoFont;
	sf::Music m_gameMusic;
    
    std::shared_ptr<gbh::SpriteNode> m_ship;
    
    std::shared_ptr<gbh::TextNode> m_timer;
    double m_timerValue = 0.0;
};
