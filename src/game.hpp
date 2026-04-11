#pragma once
#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Game {
public:
    
    void update(sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

    void destroy(Entity entity);

    void updateDeltaTime(float deltaTime);

    void createPlayer();

    void createCursor();

    void createAsteroid(uint32_t size, float vx, float vy, sf::Vector2f position);

    void asteroidSplit(Entity asteroid);

    void createBullet(sf::Angle angle, float vx, float vy, sf::Vector2f position);

    bool isRoundOver();

    void setRoundOver(bool value);

    void playSound(sf::SoundBuffer buffer);

    sf::SoundBuffer getSoundBuffer(const std::string& file);

    Registry& getRegistry();

private:
    float dt = 0.f;
    bool round_over = true;
    std::unordered_map<std::string, sf::SoundBuffer> sound_buffers = {
        {"shoot", sf::SoundBuffer("media/sounds/pistol_shot.ogg")}
    };
    sf::Sound game_sound = sf::Sound(sound_buffers["shoot"]);

    Registry registry;

    SpriteSystem spriteSystem;
    TransformSystem transformSystem;
    MovementSystem movementSystem;
    InputSystem inputSystem;
    AimSystem aimSystem;
    ShootingSystem shootingSystem;
    CollisionSystem collisionSystem;
    SpawnSystem spawnSystem;
    RoundSystem roundSystem;

};