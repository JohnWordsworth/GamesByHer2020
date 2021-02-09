#pragma once
#include "sfml-engine/scene.h"

class CreditScene : public gbh::Scene
{
protected:
    void onInitializeScene() override;
    virtual void onMouseEvent(sf::Event& event) override;
    
private:
    sf::Font m_orbitronFont;
};
