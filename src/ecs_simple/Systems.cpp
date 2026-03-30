#include "Systems.hpp"
#include "Registry.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "../game.hpp"
#include <math.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

////////////////////////////////////////////////////////////////////////////////////
//SPRITE SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

void SpriteSystem::update(Registry& reg){
        std::unordered_map<Entity, Sprite>& sprites = reg.getComponent<Sprite>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        for (int e = 1; e <= reg.maxEntity(); e++){
            if (sprites.contains(e) && transforms.contains(e)){
                sprites[e].sprite.setScale({2.f, 2.f});

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

////////////////////////////////////////////////////////////////////////////////////
//COLLISION SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

void CollisionSystem::update_hitbox(Registry& reg) {
    std::unordered_map<Entity, CollisionBox>& collision_boxes = reg.getComponent<CollisionBox>();
    std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

    for (int e = 1; e <= reg.maxEntity(); e++) {
        if (collision_boxes.contains(e) && transforms.contains(e)) {
            float shape_radius = collision_boxes[e].collision_shape.getRadius();

            collision_boxes[e].collision_shape.setRadius(transforms[e].size_x); // Assuming size_x is the diameter of the collision box
            collision_boxes[e].collision_shape.setOrigin({shape_radius, shape_radius});
            collision_boxes[e].collision_shape.setPosition(transforms[e].position);
            collision_boxes[e].collision_shape.setRotation(transforms[e].rotation);

        }
    }
}


void CollisionSystem::update_bulletcollisions(Registry& reg) {
    std::unordered_map<Entity, CollisionBox>& collision_boxes = reg.getComponent<CollisionBox>();
    std::unordered_map<Entity, BulletTag>& bulletTags = reg.getComponent<BulletTag>();
    std::unordered_map<Entity, AsteroidTag>& asteroidTags = reg.getComponent<AsteroidTag>();

    std::vector<Entity> entities_to_kill;

    for (const auto& [bullet, _] : bulletTags) {
        //std::cout << "Checking bullet: " << bullet << std::endl;
        sf::CircleShape bullet_hitbox = collision_boxes[bullet].collision_shape;
        for (const auto& [asteroid, _] : asteroidTags) {
            //std::cout << "checking asteroid: " << asteroid << std::endl;
            sf::CircleShape asteroid_hitbox = collision_boxes[asteroid].collision_shape;
            sf::Vector2f asteroid_coords = asteroid_hitbox.getPosition();
            sf::Vector2f bullet_coords = bullet_hitbox.getPosition();

            float dx = asteroid_coords.x - bullet_coords.x;
            float dy = asteroid_coords.y - bullet_coords.y;

            float distance = sqrt(pow(dx, 2) + pow(dy, 2));

            if (distance < asteroid_hitbox.getRadius() + bullet_hitbox.getRadius()){
                std::cout << "collision happening between bullet entity " << bullet << " and asteroid entity " << asteroid << std::endl;
                bool bullet_in_killqueue = false;
                bool asteroid_in_killqueue = false;

                if (std::find(entities_to_kill.begin(), entities_to_kill.end(), bullet) == entities_to_kill.end()){
                    entities_to_kill.push_back(bullet);
                }

                if (std::find(entities_to_kill.begin(), entities_to_kill.end(), asteroid) == entities_to_kill.end()){
                    entities_to_kill.push_back(asteroid);
                }
                
                
            }
        }
    }

    for (Entity e : entities_to_kill) {
        std::cout << "killing entity " << e << std::endl;
        reg.destroy(e);
    }
}


void CollisionSystem::render(Registry& reg, sf::RenderWindow& window) {
    std::unordered_map<Entity, CollisionBox>& collision_boxes = reg.getComponent<CollisionBox>();

    for (int e = 1; e <= reg.maxEntity(); e++) {
        if (collision_boxes.contains(e)) {
            window.draw(collision_boxes[e].collision_shape);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//TRANSFORM SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////
//INPUT SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

void MovementSystem::update(Registry& reg, float dt) {
        std::unordered_map<Entity, Input>& inputs = reg.getComponent<Input>();
        std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();

        const float acceleration = 250.f; // Adjust this value for faster/slower acceleration
        const float friction = 0.03f; // Adjust this value for more/less friction

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
                //std::cout << "mouse cooredinates: " << direction.x << ", " << direction.y << std::endl;
                sf::Angle angle = sf::degrees((atan2(direction.y, direction.x) * 180 / 3.14159f)); // Convert to degrees
                //std::cout << "Angle: " << angle.asDegrees() << std::endl;
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
                        //spawnSystem.createBullet(reg, transforms[e].rotation, velocity_x, velocity_y, transforms[e].position);
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

////////////////////////////////////////////////////////////////////////////////////
//SPAWNING SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

void SpawnSystem::createPlayer(Registry& reg) {
    Entity player = reg.create();
    reg.getComponent<Transform>()[player] = Transform{sf::degrees(0.0f), 0.f, 0.f, sf::Vector2f(400.f, 400.f), 200.f, 200.f, 32.f, 32.f};
    reg.getComponent<Sprite>()[player] = Sprite{};
    reg.getComponent<Input>()[player] = Input{false, false, false, false, false, false, false};
    reg.getComponent<PlayerTag>()[player] = PlayerTag{3};

    reg.getComponent<Sprite>()[player].sprite.setScale({2.f, 2.f});

    reg.getComponent<CollisionBox>()[player].collision_shape.setPosition(sf::Vector2f(400.f, 400.f));
    reg.getComponent<CollisionBox>()[player].collision_shape.setFillColor(sf::Color::Transparent);
    reg.getComponent<CollisionBox>()[player].collision_shape.setOutlineColor(sf::Color::Red);
    reg.getComponent<CollisionBox>()[player].collision_shape.setOutlineThickness(2.f);
}


void SpawnSystem::createAsteroid(Registry& reg, float vx, float vy, sf::Vector2f position) {
    sf::Texture asteroid_texture = sf::Texture("media/sprites/asteroid_large.png", false, sf::IntRect());

    Entity asteroid = reg.create();
    reg.getComponent<Transform>()[asteroid] = Transform{sf::degrees(0.0f), vx, vy, position, 200.f, 200.f, 64.f, 64.f};
    reg.getComponent<Sprite>()[asteroid] = Sprite{.texture = asteroid_texture};
    reg.getComponent<AsteroidTag>()[asteroid] = AsteroidTag{0, 100.f};

    reg.getComponent<Sprite>()[asteroid].sprite.setScale({2.f, 2.f});

    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setPosition(position);
    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setRadius(reg.getComponent<Transform>()[asteroid].size_x / 2.f); // Set radius to match the size of the asteroid sprite
    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setFillColor(sf::Color::Transparent);
    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setOutlineColor(sf::Color::Green);
    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setOutlineThickness(2.f);

}


void SpawnSystem::manageAsteroids(Registry& reg, sf::RenderWindow& window, Game& game, uint32_t maxAsteroids) {
    std::unordered_map<Entity, AsteroidTag>& asteroids = reg.getComponent<AsteroidTag>();
    //std::cout << "number of asteroids alive: " << asteroids.size() << std::endl;
    sf::Vector2u window_size = window.getSize();

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> sides(0, 3);
    std::uniform_real_distribution<float> x_coord(0.f, static_cast<float>(window_size.x));
    std::uniform_real_distribution<float> y_coord(0.f, static_cast<float>(window_size.y));

    int nsew = sides(rng); // 0 = north, 1 = south, 2 = east, 3 = west
    float asteroid_x = x_coord(rng);
    float asteroid_y = y_coord(rng);

    if (asteroids.size() < maxAsteroids) {
        switch (nsew) {
            case 0: asteroid_y = 0.f; break; // North
            case 1: asteroid_y = static_cast<float>(window_size.y); break; // South
            case 2: asteroid_x = static_cast<float>(window_size.x); break; // East
            case 3: asteroid_x = 0.f; break; // West
        }

        game.createAsteroid(0.f, 0.f, {asteroid_x, asteroid_y});
    }

}


void SpawnSystem::createCursor(Registry& reg) {
    Entity cursor = reg.create();
    reg.getComponent<CursorTag>()[cursor] = CursorTag{sf::Mouse::getPosition()};
}


void SpawnSystem::createBullet(Registry& reg, sf::Angle angle, float vx, float vy, sf::Vector2f position) {
        Entity bullet = reg.create();
        reg.getComponent<Transform>()[bullet] = Transform{angle, vx, vy, position, 5000.f, 800.f, 8.f, 8.f};
        reg.getComponent<Sprite>()[bullet] = Sprite{};
        reg.getComponent<BulletTag>()[bullet] = BulletTag{};
        reg.getComponent<CollisionBox>()[bullet] = CollisionBox{};

        //std::unordered_map<Entity, Sprite>& sprite = reg.getComponent<Sprite>()[bullet];
        reg.getComponent<Sprite>()[bullet].texture.loadFromFile("media/sprites/bullet.png");
        reg.getComponent<Sprite>()[bullet].sprite.setTexture(reg.getComponent<Sprite>()[bullet].texture);


        reg.getComponent<CollisionBox>()[bullet].collision_shape.setPosition(position);
        reg.getComponent<CollisionBox>()[bullet].collision_shape.setFillColor(sf::Color::Transparent);
        reg.getComponent<CollisionBox>()[bullet].collision_shape.setOutlineColor(sf::Color::Blue);
        reg.getComponent<CollisionBox>()[bullet].collision_shape.setOutlineThickness(2.f);
}