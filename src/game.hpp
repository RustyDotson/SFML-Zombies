#pragma once
#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Game {
public:
    
    void update(sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

    void destroy(Entity&  entity);

    void updateDeltaTime(float deltaTime);

    void createPlayer();

    void createCursor();

    void createAsteroid(float vx, float vy, sf::Vector2f position);

    void createBullet(sf::Angle angle, float vx, float vy, sf::Vector2f position);

    Registry& getRegistry();

private:
    float dt = 0.f;
    Registry registry;

    SpriteSystem spriteSystem;
    TransformSystem transformSystem;
    MovementSystem movementSystem;
    InputSystem inputSystem;
    AimSystem aimSystem;
    ShootingSystem shootingSystem;
    CollisionSystem collisionSystem;
    SpawnSystem spawnSystem;

};