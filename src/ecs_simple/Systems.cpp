#include "Systems.hpp"
#include "Registry.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "../game.hpp"
#include <math.h>


void SpriteSystem::update(Registry& reg, float dt){
        std::unordered_map<Entity, Sprite>& sprites = reg.getComponent<Sprite>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        for (int e = 1; e <= reg.maxEntity(); e++){
            if (sprites.contains(e) && transforms.contains(e)){
                sprites[e].shape.setPosition(transforms[e].position);
                sprites[e].shape.setRotation(transforms[e].rotation);

                sf::Vector2f shape_center = sprites[e].shape.getSize(); 
                sprites[e].shape.setOrigin({shape_center.x / 2.f, shape_center.y / 2.f});

                sf::Vector2 sprite_center = sprites[e].sprite.getTexture().getSize();
                sprites[e].sprite.setOrigin({sprite_center.x / 2.f, sprite_center.y / 2.f});
                sprites[e].sprite.setPosition(sf::Vector2f(transforms[e].position.x, transforms[e].position.y));
                sprites[e].sprite.setRotation(transforms[e].rotation);
            }
        }
}


void SpriteSystem::render(Registry& reg, sf::RenderWindow& window) {
    std::unordered_map<Entity, Sprite>& sprites = reg.getComponent<Sprite>();

    for (int e = 1; e <= reg.maxEntity(); e++) {
        if (sprites.contains(e)) {
            //window.draw(sprites[e].shape);
            sprites[e].sprite.setTexture(sprites[e].texture);
            window.draw(sprites[e].sprite);
        }
    }
}


void TransformSystem::update(Registry& reg, float dt) {
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


void MovementSystem::update(Registry& reg, float dt) {
        std::unordered_map<Entity, Input>& inputs = reg.getComponent<Input>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        const float acceleration = 100.f; // Adjust this value for faster/slower acceleration
        const float friction = 0.998f; // Adjust this value for more/less friction

        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (inputs.contains(e) && transforms.contains(e)){

                if (inputs[e].up) {
                    transforms[e].velocity_y -= acceleration * dt;
                }
                if (inputs[e].down) {
                    transforms[e].velocity_y += acceleration * dt;
                }
                if (inputs[e].left) {
                    transforms[e].velocity_x -= acceleration * dt;
                }
                if (inputs[e].right) {
                    transforms[e].velocity_x += acceleration * dt;
                }

                if (!inputs[e].up && !inputs[e].down) {
                    transforms[e].velocity_y *= pow(friction, dt); // Friction
                }
                if (!inputs[e].left && !inputs[e].right) {
                    transforms[e].velocity_x *= pow(friction, dt); // Friction
                }
            }
        }
}


void AimSystem::update(Registry& reg, float dt) {
        std::unordered_map<Entity, Input>& inputs = reg.getComponent<Input>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        for (int e = 1; e <= reg.maxEntity(); e++) {
            if (inputs.contains(e) && transforms.contains(e)) {
                sf::Vector2f direction = sf::Vector2f(inputs[e].mouse_position) - transforms[e].position;
                std::cout << "mouse cooredinates: " << direction.x << ", " << direction.y << std::endl;
                sf::Angle angle = sf::degrees((atan2(direction.y, direction.x) * 180 / 3.14159f)); // Convert to degrees
                std::cout << "Angle: " << angle.asDegrees() << std::endl;
                transforms[e].rotation = angle; // Adjust for sprite orientation
            }
        }
}


void ShootingSystem::update(Registry& reg, Game& game, float dt) {
    std::unordered_map<Entity, Input>& inputs = reg.getComponent<Input>();
    std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();
    std::unordered_map<Entity, PlayerTag>& playerTags = reg.getComponent<PlayerTag>();
    std::unordered_map<Entity, CursorTag>& cursorTags = reg.getComponent<CursorTag>();

    for (int e = 1; e <= reg.maxEntity(); e++) {
        if (inputs.contains(e) && transforms.contains(e)) {
            if (playerTags.contains(e)) {
                if (inputs[e].shoot) {
                    playerTags[e].timeSinceLastShot += dt;
                    if (playerTags[e].timeSinceLastShot >= playerTags[e].fireRate) {
                        playerTags[e].timeSinceLastShot = 0.f; // Reset the timer
                        float velocity_x = cos(transforms[e].rotation.asRadians()) * 500.f; // Adjust bullet speed
                        float velocity_y = sin(transforms[e].rotation.asRadians()) * 500.f;
                        game.createBullet(transforms[e].rotation, velocity_x, velocity_y, transforms[e].position);
                    } 
                    else {
                        continue; // Skip shooting if fire rate not met
                    } 
                }
            }
        }
    }

    // Update bullets separately to avoid iterator issues during erase
    std::unordered_map<Entity, BulletTag>& bulletTags = reg.getComponent<BulletTag>();
    std::vector<Entity> bulletsToDestroy;
    
    for (auto& [e, bullet] : bulletTags) {
        bullet.timeAlive += dt;
        if (bullet.timeAlive >= bullet.lifetime) {
            bulletsToDestroy.push_back(e);
        }
    }
    
    for (Entity e : bulletsToDestroy) {
        reg.destroy(e);
    }
}


void InputSystem::update(Registry& reg, sf::RenderWindow& window) {
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
                inputs[e].shoot = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
                inputs[e].look_right = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
                inputs[e].look_left = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
                inputs[e].mouse_position = sf::Mouse::getPosition(window);
            }
        }
}