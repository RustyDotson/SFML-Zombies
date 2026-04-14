#include "Registry.hpp"
#pragma once

class Game;
struct SoundManager;
struct StatsManager;
struct UIManager;

struct SpriteSystem{
    void update(Registry& reg);
    void render(Registry& reg, sf::RenderWindow& window);
};

struct CollisionSystem{
    void update_hitbox(Registry& reg);
    void update_bulletcollisions(Registry& reg, Game& game);
    void update_playercollisions(Registry& reg, Game& game);
    void render(Registry& reg, sf::RenderWindow& window);
};


struct TransformSystem {
    void update(Registry& reg, float dt);
    void asteroidScreenWrap(Registry& reg, sf::Vector2u window_size);
    void asteroidRotation(Registry& reg, float dt);
    void ScreenWrap(Registry& reg, sf::Vector2u window_size);
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
    void createAsteroid(Registry& reg, uint32_t size, float vx, float vy, sf::Vector2f position);
    void createCursor(Registry& reg);
    void createBullet(Game& game, Registry& reg, sf::Angle angle, float vx, float vy, sf::Vector2f position);

    void asteroidSplit(Registry& reg, Entity asteroid);
};

struct RoundSystem {
    void newRound(Registry& reg, sf::RenderWindow& window, Game& game, uint32_t& maxAsteroids);
};

struct UISystem {
    void update(Registry& reg, sf::RenderWindow& window, Game& game);
    void render(Registry& reg, sf::RenderWindow& window, Game& game);
};