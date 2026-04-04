#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#pragma once

struct Velocity {
    float vx, vy;
};

struct Transform {
    sf::Angle rotation; 
    float velocity_x, velocity_y;
    sf::Vector2f position;
    float max_speed;
    float max_rotation_speed;
    float size_x, size_y;
};

struct Health {
    int hp;
};

struct Sprite {
    sf::Texture texture = sf::Texture("media/sprites/ship.png", false, sf::IntRect());
    sf::Sprite sprite = sf::Sprite(texture);
};

struct CollisionBox {
    sf::CircleShape collision_shape = sf::CircleShape(20.f); // Default radius, can be set per entity
};

struct Input {
    bool up, down, left, right, shoot, look_right, look_left;
    sf::Vector2i mouse_position;
};

struct Lifetime {
    float timeLeft;
};

struct PlayerTag {
    uint32_t lives = 3;
    float fireRate = 0.1f;
    float timeSinceLastShot = 10.f;
    float speed = 200.f;
};

struct CursorTag {
    sf::Vector2i position;
};

struct BulletTag {
    float lifetime = 3.f;
    float timeAlive = 0.f;
};

struct AsteroidTag {
    uint32_t size = 0; // 0 = large, 1 = medium, 2 = small
    float speed = 100.f;
    float rotation_speed = 0.f;
    std::unordered_map<uint32_t, float> size_to_pixels = {
        {0, 64.f},
        {1, 32.f},
        {2, 16.f}
    };

    std::unordered_map<uint32_t, std::string> size_to_texture = {
        {0, "media/sprites/asteroid_large.png"},
        {1, "media/sprites/asteroid_medium.png"},
        {2, "media/sprites/asteroid_small.png"}
    };
};