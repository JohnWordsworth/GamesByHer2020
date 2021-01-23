#pragma once 

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>


class TitleScene : public gbh::Scene
{
protected:
	void onInitializeScene() override;

	virtual void onUpdate(double deltaTime);
	void onMouseEvent(sf::Event& event) override;
	virtual void onShowScene();
	virtual void onHideScene();

private:
	sf::Font m_robotoFont;
	sf::Music m_titleMusic;
};
