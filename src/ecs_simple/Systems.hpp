#include "Registry.hpp"
#pragma once

class Game;

struct SpriteSystem{
    void update(Registry& reg);
    void render(Registry& reg, sf::RenderWindow& window);
};

struct CollisionSystem{
    void update_hitbox(Registry& reg);
    void update_bulletcollisions(Registry& reg);
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

struct SpawnSystem {
    void createPlayer(Registry& reg);
    void createAsteroid(Registry& reg);
    void createCursor(Registry& reg);
    void createBullet(Registry& reg, sf::Angle angle, float vx, float vy, sf::Vector2f position);
};

struct SoundSystem {
    void playSound(const std::string& soundFile);
};