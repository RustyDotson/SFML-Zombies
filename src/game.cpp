#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Components.hpp"
#include "ecs_simple/Entity.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "game.hpp"

 
void Game::update(sf::RenderWindow& window) {
    inputSystem.update(registry, window);
    movementSystem.update(registry, dt);
    transformSystem.update(registry, dt);
    aimSystem.update(registry, dt);
    shootingSystem.update(registry, *this, dt);
    collisionSystem.update(registry);
    spriteSystem.update(registry);
}

void Game::render(sf::RenderWindow& window) {
    window.clear();
    spriteSystem.render(registry, window);
    collisionSystem.render(registry, window);
    window.display();
}

void Game::destroy(Entity entity) {
    // Clean up resources if needed
    registry.destroy(entity);
}

void Game::updateDeltaTime(float deltaTime) {
    dt = deltaTime;
}

void Game::createPlayer() {
    Entity player = registry.create();
    registry.getComponent<Transform>()[player] = Transform{sf::degrees(0.0f), 0.f, 0.f, sf::Vector2f(400.f, 400.f), 200.f, 200.f, 32.f, 32.f};
    registry.getComponent<Sprite>()[player] = Sprite{};
    registry.getComponent<Input>()[player] = Input{false, false, false, false, false, false, false};
    registry.getComponent<PlayerTag>()[player] = PlayerTag{3};

    registry.getComponent<Sprite>()[player].sprite.setScale({2.f, 2.f});

    registry.getComponent<CollisionBox>()[player].collision_shape.setPosition(sf::Vector2f(400.f, 400.f));
    registry.getComponent<CollisionBox>()[player].collision_shape.setFillColor(sf::Color::Transparent);
    registry.getComponent<CollisionBox>()[player].collision_shape.setOutlineColor(sf::Color::Red);
    registry.getComponent<CollisionBox>()[player].collision_shape.setOutlineThickness(2.f);
}

void Game::createAsteroid() {
    sf::Texture asteroid_texture = sf::Texture("media/sprites/asteroid_large.png", false, sf::IntRect());

    Entity asteroid = registry.create();
    registry.getComponent<Transform>()[asteroid] = Transform{sf::degrees(0.0f), 0.f, 0.f, sf::Vector2f(400.f, 400.f), 200.f, 200.f, 64.f, 64.f};
    registry.getComponent<Sprite>()[asteroid] = Sprite{.texture = asteroid_texture};
    registry.getComponent<AsteroidTag>()[asteroid] = AsteroidTag{0, 100.f};

    registry.getComponent<Sprite>()[asteroid].sprite.setScale({2.f, 2.f});

    registry.getComponent<CollisionBox>()[asteroid].collision_shape.setPosition(sf::Vector2f(400.f, 400.f));
    registry.getComponent<CollisionBox>()[asteroid].collision_shape.setRadius(registry.getComponent<Transform>()[asteroid].size_x / 2.f); // Set radius to match the size of the asteroid sprite
    registry.getComponent<CollisionBox>()[asteroid].collision_shape.setFillColor(sf::Color::Transparent);
    registry.getComponent<CollisionBox>()[asteroid].collision_shape.setOutlineColor(sf::Color::Green);
    registry.getComponent<CollisionBox>()[asteroid].collision_shape.setOutlineThickness(2.f);

}

void Game::createCursor() {
    Entity cursor = registry.create();
    registry.getComponent<CursorTag>()[cursor] = CursorTag{sf::Mouse::getPosition()};
}

void Game::createBullet(sf::Angle angle, float vx, float vy, sf::Vector2f position) {
        Entity bullet = registry.create();
        registry.getComponent<Transform>()[bullet] = Transform{angle, vx, vy, position, 5000.f, 800.f, 8.f, 8.f};
        registry.getComponent<Sprite>()[bullet] = Sprite{};
        registry.getComponent<BulletTag>()[bullet] = BulletTag{};
        registry.getComponent<CollisionBox>()[bullet] = CollisionBox{};

        //std::unordered_map<Entity, Sprite>& sprite = registry.getComponent<Sprite>()[bullet];
        registry.getComponent<Sprite>()[bullet].texture.loadFromFile("media/sprites/bullet.png");
        registry.getComponent<Sprite>()[bullet].sprite.setTexture(registry.getComponent<Sprite>()[bullet].texture);


        registry.getComponent<CollisionBox>()[bullet].collision_shape.setPosition(position);
        registry.getComponent<CollisionBox>()[bullet].collision_shape.setFillColor(sf::Color::Transparent);
        registry.getComponent<CollisionBox>()[bullet].collision_shape.setOutlineColor(sf::Color::Blue);
        registry.getComponent<CollisionBox>()[bullet].collision_shape.setOutlineThickness(2.f);
}

Registry& Game::getRegistry() {
    return registry;
}