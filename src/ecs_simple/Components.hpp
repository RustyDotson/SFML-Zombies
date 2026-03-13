#include <SFML/Graphics.hpp>
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
};

struct Health {
    int hp;
};

struct Sprite {
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(50.f, 50.f));
    sf::Texture texture = sf::Texture("media/sprites/ship.png", false, sf::IntRect({10, 10}, {32, 32}));
    sf::Sprite sprite = sf::Sprite(texture);
};

struct Shape {
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(50.f, 50.f));
};

struct Collision {
    float radius;
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
};

struct CursorTag {
    sf::Vector2i position;
};

struct BulletTag {};
struct AsteroidTag {};