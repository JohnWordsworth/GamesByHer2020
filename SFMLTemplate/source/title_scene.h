#pragma once
#include "asteroid.h"
#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>

class TitleScene : public gbh::Scene
{
protected:
    void onInitializeScene() override;

    virtual void onUpdate(double deltaTime) override;
    virtual void onMouseEvent(sf::Event& event) override;
    virtual void onShowScene() override;
    virtual void onHideScene() override;
    
    void createAsteroids();
private:
    std::shared_ptr<gbh::SpriteNode> m_asteroid01;
    std::shared_ptr<gbh::SpriteNode> m_asteroid02;
    std::shared_ptr<gbh::SpriteNode> m_asteroid03;
    std::shared_ptr<gbh::SpriteNode> m_asteroid04;
    
    std::vector<std::shared_ptr<gbh::SpriteNode>> m_asteroids;

    sf::Font m_orbitronFont;
    sf::Music m_titleMusic;
};
