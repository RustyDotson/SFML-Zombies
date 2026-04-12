#pragma once
#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include "ecs_simple/Managers.hpp"

class Game {
public:
    
    //Game();

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

    void playSound(const std::string& soundFile);

    void randomSoundPitch(const std::string& soundFile);

    sf::SoundBuffer getSoundBuffer(const std::string& file);

    void updateStat(const std::string stat_name, std::string message, int amount);

    Registry& getRegistry();

private:
    float dt = 0.f;
    bool round_over = true;

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

    SoundManager soundManager;
    StatsManager statsManager;
    UIManager uiManager;
    //SoundSystem soundSystem;
};