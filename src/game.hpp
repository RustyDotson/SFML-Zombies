#pragma once
#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Components.hpp"
#include "ecs_simple/Entity.hpp"
#include "ecs_simple/Systems.hpp"

class Game {
public:
    
    void update() {
        inputSystem.update(registry);
        movementSystem.update(registry);
        transformSystem.update(registry, dt);
        spriteSystem.update(registry);
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
        registry.getComponent<Transform>()[player] = Transform{0.f, 0.f, 0.f, sf::Vector2f(400.f, 300.f)};
        registry.getComponent<Sprite>()[player] = Sprite{};
        registry.getComponent<Input>()[player] = Input{false, false, false, false, false};
    }

    Registry& getRegistry() {
        return registry;
    }

private:
    float dt;
    Registry registry;

    SpriteSystem spriteSystem;
    TransformSystem transformSystem;
    MovementSystem movementSystem;
    InputSystem inputSystem;
};