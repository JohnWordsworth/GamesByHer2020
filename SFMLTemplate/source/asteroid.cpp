#include "asteroid.h"
#include <iostream>
#include <random>        // mt19937, uniform_int_distribution
#include <ctime>        // time
#include <cmath>

std::mt19937 randomiser = std::mt19937(std::time(nullptr));
std::vector<std::string> asteroidImages = {
    "../assets/gfx/asteroid-medium-02.png",
    "../assets/gfx/asteroid-small-02.png",
    "../assets/gfx/asteroid-small-01.png"
};

std::vector<std::shared_ptr<gbh::SpriteNode>> Asteroid::createAsteroids()
{
    std::vector<std::shared_ptr<gbh::SpriteNode>> tempVector;
    std::uniform_int_distribution<int> xRandom(40, 1240);
    std::uniform_int_distribution<int> yRandom(40, 680);

    for(int i = 0; i < 6; ++i)
    {
        std::shuffle(asteroidImages.begin(), asteroidImages.end(), randomiser);

        float x = xRandom(randomiser);
        float y = yRandom(randomiser);

        // Avoid middle 100x100 box where the player ship is
        while (abs((1280/2)-x) < 50 && abs((720/2)-y) < 50) {
            x = xRandom(randomiser);
            y = yRandom(randomiser);
        }
                
        auto asteroid = std::make_shared<gbh::SpriteNode>(asteroidImages.front());
        asteroid->setPosition(x, y);
        asteroid->setOrigin(0.5f,0.5f);
        
        tempVector.push_back(asteroid);
    }
    
    return tempVector;
}
