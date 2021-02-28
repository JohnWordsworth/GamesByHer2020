#pragma once

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>

class LevelSelectScene : public gbh::Scene
{
protected:
    void onInitializeScene() override;
    virtual void onMouseEvent(sf::Event& event) override;

private:
    sf::Font m_orbitronFont;
    sf::Music m_titleMusic;
};
