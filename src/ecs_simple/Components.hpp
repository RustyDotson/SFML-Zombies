#include <SFML/Graphics.hpp>
#pragma once

struct Velocity {
    float vx, vy;
};

struct Transform {
    float rotation; 
    float velocity_x, velocity_y;
    sf::Vector2f position;
};

struct Health {
    int hp;
};

struct Sprite {
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(50.f, 50.f));
};

struct Collision {
    float radius;
};

struct Input {
    bool up, down, left, right, shoot;
};

struct Lifetime {
    float timeLeft;
};

struct PlayerTag {};
struct BulletTag {};
struct AsteroidTag {};