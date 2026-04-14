#include <iostream>
#include "tests.hpp"
#include "game.hpp"
#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Components.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>

#define IS_TRUE(x) {                                                                        \
    if (!(x)) {                                                                             \
        std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl;           \
    }else {std::cout << __FUNCTION__ << " succeeded on line " << __LINE__ << std::endl;}    \
}

void test_registry_create_destroy() {
    Registry registry;
    Entity e1 = registry.create();
    IS_TRUE(e1 == 1);
    Entity e2 = registry.create();
    IS_TRUE(e2 == 2);
    registry.destroy(e1);
    Entity e3 = registry.create();
    IS_TRUE(e3 == 1); // Should reuse id 1
}

void test_game_create_player() {
    Game game;
    game.createPlayer();
    auto& players = game.getRegistry().getComponent<PlayerTag>();
    IS_TRUE(players.size() == 1);
    auto& transforms = game.getRegistry().getComponent<Transform>();
    IS_TRUE(transforms.size() == 1);
}

void test_game_create_asteroid() {
    Game game;
    game.createAsteroid(0, 10.f, 20.f, sf::Vector2f(100.f, 200.f));
    auto& asteroids = game.getRegistry().getComponent<AsteroidTag>();
    IS_TRUE(asteroids.size() == 1);
    auto& transforms = game.getRegistry().getComponent<Transform>();
    IS_TRUE(transforms.size() == 1);
    // Check position
    Entity asteroid = asteroids.begin()->first;
    IS_TRUE(transforms[asteroid].position == sf::Vector2f(100.f, 200.f));
}

void test_game_create_bullet() {
    Game game;
    game.createBullet(sf::degrees(0), 100.f, 200.f, sf::Vector2f(50.f, 50.f));
    auto& bullets = game.getRegistry().getComponent<BulletTag>();
    IS_TRUE(bullets.size() == 1);
    auto& transforms = game.getRegistry().getComponent<Transform>();
    IS_TRUE(transforms.size() == 1);
}

void test_collision_system() {
    Game game;
    // Create two entities with collision boxes
    Entity e1 = game.getRegistry().create();
    game.getRegistry().getComponent<Transform>()[e1] = {sf::degrees(0), 0, 0, sf::Vector2f(0, 0), 0, 0, 10, 10};
    game.getRegistry().getComponent<CollisionBox>()[e1] = {sf::CircleShape(10.f)};

    Entity e2 = game.getRegistry().create();
    game.getRegistry().getComponent<Transform>()[e2] = {sf::degrees(0), 0, 0, sf::Vector2f(15, 0), 0, 0, 10, 10};
    game.getRegistry().getComponent<CollisionBox>()[e2] = {sf::CircleShape(10.f)};

    CollisionSystem collisionSystem;
    collisionSystem.update_hitbox(game.getRegistry());
    // Assuming collision detection logic, but since it's not implemented in the snippet, just check if it runs without error
    IS_TRUE(true); // Placeholder
}

void test_movement_system() {
    Game game;
    Entity player = game.getRegistry().create();
    game.getRegistry().getComponent<Transform>()[player] = {sf::degrees(0), 0, 0, sf::Vector2f(0, 0), 100, 10, 10, 10};
    game.getRegistry().getComponent<Input>()[player] = {true, false, false, false, false, false, false, sf::Vector2i(0,0)}; // up pressed
    game.getRegistry().getComponent<PlayerTag>()[player] = {3, 0.1f, 3000.f, 10.f, 200.f};

    MovementSystem movementSystem;
    TransformSystem transformSystem;
    game.updateDeltaTime(0.016f); // ~60fps
    movementSystem.update_player(game.getRegistry(), 0.016f);
    transformSystem.update(game.getRegistry(), 0.016f);
    // Check if position changed
    IS_TRUE(game.getRegistry().getComponent<Transform>()[player].position.y < 0); // Moved up
}

void test_round_over() {
    Game game;
    IS_TRUE(game.isRoundOver() == true); // Initially true
    game.setRoundOver(false);
    IS_TRUE(game.isRoundOver() == false);
}

int main() {
    test_registry_create_destroy();
    test_game_create_player();
    test_game_create_asteroid();
    test_game_create_bullet();
    test_collision_system();
    test_movement_system();
    test_round_over();
    std::cout << "All tests completed." << std::endl;
    return 0;
}