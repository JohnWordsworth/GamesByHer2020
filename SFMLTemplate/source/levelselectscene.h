#pragma once

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>

class LevelSelectScene : public gbh::Scene
{
public:
    std::shared_ptr<int> m_levelSelected = nullptr;

protected:
    void onInitializeScene() override;
    virtual void onMouseEvent(sf::Event& event) override;

private:
    sf::Font m_orbitronFont;
    sf::Music m_titleMusic;
};
