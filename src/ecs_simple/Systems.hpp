#include "Registry.hpp"
#pragma once

class Game;

struct SpriteSystem{
    void update(Registry& reg, float dt);
    void render(Registry& reg, sf::RenderWindow& window);
};


struct TransformSystem {
    void update(Registry& reg, float dt);
};


struct MovementSystem {
    void update(Registry& reg, float dt);
};


struct AimSystem {
    void update(Registry& reg, float dt);
}; 


struct ShootingSystem {
    void update(Registry& reg, Game& game, float dt);
};


struct InputSystem {
    void update(Registry& reg, sf::RenderWindow& window);
};