#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Components.hpp"
#include "ecs_simple/Entity.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "game.hpp"

 
void Game::update(sf::RenderWindow& window) {
    inputSystem.update(registry, window);
    movementSystem.update_player(registry, dt);
    
    aimSystem.update(registry, dt);
    //shootingSystem.update(registry, *this, dt);
    shootingSystem.update(registry, *this, dt);
    
    spawnSystem.manageAsteroids(registry, window, *this, 500);
    collisionSystem.update_hitbox(registry);
    collisionSystem.update_bulletcollisions(registry);
    
    
    transformSystem.update(registry, dt);
    spriteSystem.update(registry);
}

void Game::render(sf::RenderWindow& window) {
    window.clear();
    spriteSystem.render(registry, window);
    //collisionSystem.render(registry, window);
    window.display();
}

void Game::destroy(Entity& entity) {
    // Clean up resources if needed
    registry.destroy(entity);
}

void Game::updateDeltaTime(float deltaTime) {
    dt = deltaTime;
}

void Game::createPlayer() {
    spawnSystem.createPlayer(registry);
}

void Game::createAsteroid(float vx, float vy, sf::Vector2f position) {
    spawnSystem.createAsteroid(registry, vx, vy, position);
}

void Game::createCursor() {
    spawnSystem.createCursor(registry);
}

void Game::createBullet(sf::Angle angle, float vx, float vy, sf::Vector2f position) {
    spawnSystem.createBullet(registry, angle, vx, vy, position);
}

Registry& Game::getRegistry() {
    return registry;
}