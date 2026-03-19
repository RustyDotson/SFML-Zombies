#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Components.hpp"
#include "ecs_simple/Entity.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>
#include "game.hpp"


    
void Game::update(sf::RenderWindow& window) {
    inputSystem.update(registry, window);
    movementSystem.update(registry, dt);
    transformSystem.update(registry, dt);
    aimSystem.update(registry, dt);
    spriteSystem.update(registry, dt);
    shootingSystem.update(registry, *this);
}

void Game::render(sf::RenderWindow& window) {
    window.clear();
    spriteSystem.render(registry, window);
    window.display();
}

    void Game::updateDeltaTime(float deltaTime) {
        dt = deltaTime;
    }

    void Game::createPlayer() {
        Entity player = registry.create();
        registry.getComponent<Transform>()[player] = Transform{sf::degrees(0.0f), 0.f, 0.f, sf::Vector2f(400.f, 400.f), 200.f, 200.f};
        registry.getComponent<Sprite>()[player] = Sprite{};
        registry.getComponent<Input>()[player] = Input{false, false, false, false, false, false, false};
        registry.getComponent<PlayerTag>()[player] = PlayerTag{3};

        registry.getComponent<Sprite>()[player].sprite.setScale({1.5f, 1.5f});
    }

    void Game::createCursor() {
        Entity cursor = registry.create();
        registry.getComponent<CursorTag>()[cursor] = CursorTag{sf::Mouse::getPosition()};
    }

    void Game::createBullet(sf::Angle angle, sf::Vector2f position) {
            Entity bullet = registry.create();
            registry.getComponent<Transform>()[bullet] = Transform{angle, 200.f, 200.f, position, 200.f, 200.f};
            registry.getComponent<Sprite>()[bullet] = Sprite{};
            registry.getComponent<BulletTag>()[bullet] = BulletTag{};
    

            registry.getComponent<Sprite>()[bullet].texture.loadFromFile("media/sprites/bullet.png");
            registry.getComponent<Sprite>()[bullet].sprite.setScale({1.5f, 1.5f});
            registry.getComponent<Sprite>()[bullet].sprite.setTexture(registry.getComponent<Sprite>()[bullet].texture);
    }

    Registry& Game::getRegistry() {
        return registry;
    }