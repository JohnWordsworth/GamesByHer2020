#pragma once
#include "sfml-engine/spritenode.h"
#include "sfml-engine/physics/physicsbody.h"
#include "sfml-engine/physics/physicscontact.h"

struct Asteroid
{
public:
    static std::vector<std::shared_ptr<gbh::SpriteNode>> createAsteroids();
};
