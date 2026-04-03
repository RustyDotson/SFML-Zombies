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
    void asteroidScreenWrap(Registry& reg, sf::Vector2u window_size);
};


struct MovementSystem {
    void update_player(Registry& reg, float dt);
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
    void createAsteroid(Registry& reg, float vx, float vy, sf::Vector2f position);
    void newRound(Registry& reg, sf::RenderWindow& window, Game& game, uint32_t& maxAsteroids);
    void createCursor(Registry& reg);
    void createBullet(Registry& reg, sf::Angle angle, float vx, float vy, sf::Vector2f position);
};

struct SoundSystem {
    void playSound(const std::string& soundFile);
};

struct RoundSystem {
    void newRound(Registry& reg, sf::RenderWindow& window, Game& game, uint32_t& maxAsteroids);
};