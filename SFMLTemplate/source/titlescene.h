#pragma once 

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>


class TitleScene : public gbh::Scene
{
protected:
	void onInitializeScene() override;

	void onUpdate(double deltaTime) override;
	void onMouseEvent(sf::Event& event) override;
    void onKeyboardEvent(sf::Event& event) override;
    void onBeginPhysicsContact(const gbh::PhysicsContact& contact) override;
    
	void onShowScene() override;
	void onHideScene() override;

private:
	sf::Font m_robotoFont;
	sf::Music m_titleMusic;
    
    std::shared_ptr<gbh::SpriteNode> m_ship;
    
    std::shared_ptr<gbh::TextNode> m_timer;
    double m_timerValue = 0.0;
};
