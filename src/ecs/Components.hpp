#include <SFML/Graphics.hpp>

struct Position {
    float x, y;
};

struct Velocity {
    float vx, vy;
};

struct Transform {
    float rotation, x, y;
};

struct Health {
    int hp;
};

struct Sprite {
    sf::ConvexShape shape;
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
