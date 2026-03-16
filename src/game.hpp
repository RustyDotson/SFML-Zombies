#pragma once
#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Components.hpp"
#include "ecs_simple/Entity.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>

class Game {
public:
    
    void update(sf::RenderWindow& window) {
        inputSystem.update(registry, window);
        movementSystem.update(registry);
        transformSystem.update(registry, dt);
        aimSystem.update(registry);
        spriteSystem.update(registry, dt);
    }

    void render(sf::RenderWindow& window) {
        window.clear();
        spriteSystem.render(registry, window);
        window.display();
    }

    void updateDeltaTime(float deltaTime) {
        dt = deltaTime;
    }

    void createPlayer() {
        Entity player = registry.create();
        registry.getComponent<Transform>()[player] = Transform{sf::degrees(0.0f), 0.f, 0.f, sf::Vector2f(400.f, 300.f), 200.f, 200.f};
        registry.getComponent<Sprite>()[player] = Sprite{};
        registry.getComponent<Input>()[player] = Input{false, false, false, false, false, false, false};
        registry.getComponent<PlayerTag>()[player] = PlayerTag{3};
    }

    void createCursor() {
        Entity cursor = registry.create();
        registry.getComponent<CursorTag>()[cursor] = CursorTag{sf::Mouse::getPosition()};
    }

    Registry& getRegistry() {
        return registry;
    }

private:
    float dt = 0.f;
    Registry registry;

    SpriteSystem spriteSystem;
    TransformSystem transformSystem;
    MovementSystem movementSystem;
    InputSystem inputSystem;
    AimSystem aimSystem;
};