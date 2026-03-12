#include "Registry.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#pragma once

struct SpriteSystem{

    void update(Registry& reg){
        std::unordered_map<Entity, Sprite>& sprites = reg.getComponent<Sprite>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        for (int e = 1; e <= reg.maxEntity(); e++){
            if (sprites.contains(e) && transforms.contains(e)){
                sprites[e].shape.setPosition(transforms[e].position);
                sprites[e].shape.rotate(sf::degrees(transforms[e].rotation));
            }
        }
    }

    void render(Registry& reg, sf::RenderWindow& window) {
        std::unordered_map<Entity, Sprite>& sprites = reg.getComponent<Sprite>();

        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (sprites.contains(e)) {
                window.draw(sprites[e].shape);
            }
        }
    }
};

struct TransformSystem {
    

    void update(Registry& reg, float dt) {
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();
        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (transforms.contains(e)){
                transforms[e].position.x += transforms[e].velocity_x * dt;
                transforms[e].position.y += transforms[e].velocity_y * dt;
            }
        }
    }
};

struct MovementSystem {
    
    void update(Registry& reg) {
        std::unordered_map<Entity, Input>& inputs = reg.getComponent<Input>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (inputs.contains(e) && transforms.contains(e)){
                if (inputs[e].up) {
                    transforms[e].velocity_y -= 0.1f;
                }
                if (inputs[e].down) {
                    transforms[e].velocity_y += 0.1f;
                }
                if (inputs[e].left) {
                    transforms[e].velocity_x -= 0.1f;
                }
                if (inputs[e].right) {
                    transforms[e].velocity_x += 0.1f;
                }


                if (!inputs[e].up && !inputs[e].down) {
                    transforms[e].velocity_y *= 0.95f; // Friction
                }
                if (!inputs[e].left && !inputs[e].right) {
                    transforms[e].velocity_x *= 0.95f; // Friction
                }


                if(inputs[e].look_right) {
                    transforms[e].rotation += 0.1f; // Rotate right
                }
                if (inputs[e].look_left) {
                    transforms[e].rotation -= 0.1f; // Rotate left
                }

                if (!inputs[e].look_right && !inputs[e].look_left) {
                    // Optional: Add some rotational friction if no rotation input is given
                    transforms[e].rotation *= 0.95f;
                }
            }
        }
    }
};

struct InputSystem {


    void update(Registry& reg) {
        std::unordered_map<Entity, Input>& inputs = reg.getComponent<Input>();
        // This function would be called in the main event loop to update input states
        // For example, you could set inputs[e].up = true when the up key is pressed
        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (inputs.contains(e)) {
                // Update input states based on events
                inputs[e].up = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
                inputs[e].down = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
                inputs[e].left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
                inputs[e].right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
                inputs[e].shoot = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
                inputs[e].look_right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
                inputs[e].look_left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
            }
        }
    }
};