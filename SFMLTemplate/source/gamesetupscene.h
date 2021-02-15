#pragma once 

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>


class GameSetupScene : public gbh::Scene
{
protected:
	void onInitializeScene() override;
    void onMouseEvent(sf::Event& event) override;

    std::shared_ptr<gbh::Node> createButton(const sf::Vector2f& size, const std::string& text, const sf::Color color, const std::string& name);
private:
	sf::Font m_robotoFont;
};
