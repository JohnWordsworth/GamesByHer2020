#pragma once

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>

struct LevelSelection
{
    std::string level;
    std::string fileToLoad;
};

class LevelSelectScene : public gbh::Scene
{
protected:
    void onInitializeScene() override;
    virtual void onMouseEvent(sf::Event& event) override;

    std::shared_ptr<gbh::Node> createBtn(const sf::Vector2f &size, const std::string &txt, const std::string &name);

private:
    sf::Font m_orbitronFont;
    sf::Music m_titleMusic;
    std::vector<LevelSelection> levels;
};
