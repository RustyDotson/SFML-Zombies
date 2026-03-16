#include "Registry.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include <math.h>
#pragma once

struct SpriteSystem{

    void update(Registry& reg, float dt){
        std::unordered_map<Entity, Sprite>& sprites = reg.getComponent<Sprite>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        for (int e = 1; e <= reg.maxEntity(); e++){
            if (sprites.contains(e) && transforms.contains(e)){
                sprites[e].shape.setPosition(transforms[e].position);
                //sprites[e].shape.rotate(sf::degrees(transforms[e].rotation)*dt);
                sprites[e].shape.setRotation(transforms[e].rotation);

                //sets the point of rotation in the center of the shape.
                //float centerX = sprites[e].shape.getTexture().getSize().x;
                //float centerY = sprites[e].shape.getTexture().getSize().y;
                //sprites[e].shape.setOrigin({centerX / 2, centerY / 2}); 

                sf::Vector2f shape_center = sprites[e].shape.getSize(); 
                sprites[e].shape.setOrigin({shape_center.x / 2.f, shape_center.y / 2.f});

                sf::Vector2 sprite_center = sprites[e].sprite.getTexture().getSize();
                sprites[e].sprite.setOrigin({sprite_center.x / 2.f, sprite_center.y / 2.f});
                sprites[e].sprite.setPosition(sf::Vector2f(transforms[e].position.x, transforms[e].position.y));
                sprites[e].sprite.setRotation(transforms[e].rotation);


                //float centerX = sprites[e].shape.getTexture().getSize().x;
                //float centerY = sprites[e].shape.getTexture().getSize().y;
                //sprites[e].shape.setOrigin({centerX / 2, centerY / 2}); 
            }
        }
    }

    void render(Registry& reg, sf::RenderWindow& window) {
        std::unordered_map<Entity, Sprite>& sprites = reg.getComponent<Sprite>();

        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (sprites.contains(e)) {
                std::cout << "Sprite texture: " << sprites[e].texture.getSize().x << " x " << sprites[e].texture.getSize().y << std::endl;
                //window.draw(sprites[e].shape);
                sprites[e].sprite.setTexture(sprites[e].texture);
                window.draw(sprites[e].sprite);
            }
        }
    }
};


struct TransformSystem {
    
    void update(Registry& reg, float dt) {
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();
        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (transforms.contains(e)){
                if (transforms[e].velocity_x > transforms[e].max_speed) {transforms[e].velocity_x = transforms[e].max_speed;}
                if (transforms[e].velocity_x < -transforms[e].max_speed) {transforms[e].velocity_x = -transforms[e].max_speed;}
                if (transforms[e].velocity_y > transforms[e].max_speed) {transforms[e].velocity_y = transforms[e].max_speed;}
                if (transforms[e].velocity_y < -transforms[e].max_speed) {transforms[e].velocity_y = -transforms[e].max_speed;}

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
                    transforms[e].velocity_y *= 0.999f; // Friction
                }
                if (!inputs[e].left && !inputs[e].right) {
                    transforms[e].velocity_x *= 0.999f; // Friction
                }
            }
        }
    }
};


struct AimSystem {
    void update(Registry& reg) {
        std::unordered_map<Entity, Input>& inputs = reg.getComponent<Input>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (inputs.contains(e) && transforms.contains(e)) {
                sf::Vector2f direction = sf::Vector2f(inputs[e].mouse_position) - transforms[e].position;
                std::cout << "mouse cooredinates: " << direction.x << ", " << direction.y << std::endl;
                sf::Angle angle = sf::degrees((atan2(direction.y, direction.x) * 180 / 3.14159f) + 90.f); // Convert to degrees
                std::cout << "Angle: " << angle.asDegrees() << std::endl;
                transforms[e].rotation = angle; // Adjust for sprite orientation
            }
        }
    }
};


struct InputSystem {


    void update(Registry& reg, sf::RenderWindow& window) {
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
                inputs[e].mouse_position = sf::Mouse::getPosition(window);
            }
        }
    }
};