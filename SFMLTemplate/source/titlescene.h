#pragma once 

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>


/**
   The Title Screen that basically just has a 'start game' button but has a bunch of other
   effects to test parts of the engine.
 */
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
    
    void createBackground();
    void createAsteroids();
    void createPlayerShip();
    void createUI();

private:
	sf::Font m_robotoFont;
	sf::Music m_titleMusic;
    
    std::shared_ptr<gbh::TextNode> m_titleNode;
    std::shared_ptr<gbh::SpriteNode> m_ship;
    
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_asteroids;
    
    double m_sceneTimer = 0.0;
};
