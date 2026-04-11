#include "Systems.hpp"
#include "Registry.hpp"
#include "Components.hpp"
#include "Entity.hpp"
#include "../game.hpp"
#include <math.h>
#include <iostream>
#include <vector>
#include <random>
#include "utils/helpers.hpp"

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


void CollisionSystem::update_playercollisions(Registry& reg, Game& game) {
    // This function can be implemented to check for collisions between the player and asteroids, and handle player damage/lives accordingly.
    // Similar to update_bulletcollisions, but checks for collisions between player entities and asteroid entities.
    std::unordered_map<Entity, CollisionBox>& collision_boxes = reg.getComponent<CollisionBox>();
    std::unordered_map<Entity, AsteroidTag>& asteroidTags = reg.getComponent<AsteroidTag>();
    std::unordered_map<Entity, PlayerTag>& playerTags = reg.getComponent<PlayerTag>();

    std::vector<Entity> kill_buffer;

    for (const auto [player_entity, player] : playerTags) {
        sf::CircleShape player_hitbox = collision_boxes[player_entity].collision_shape;
        for (const auto [asteroid_entity, asteroid] : asteroidTags) {
            sf::CircleShape asteroid_hitbox = collision_boxes[asteroid_entity].collision_shape;

            if (utils::collisionCheck(player_hitbox, asteroid_hitbox)) {
                kill_buffer.push_back(player_entity);
            }
        }
    }

    for (Entity e : kill_buffer){
        game.destroy(e);
    }
}

void CollisionSystem::update_bulletcollisions(Registry& reg, Game& game) {
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
            
            if (utils::collisionCheck(asteroid_hitbox, bullet_hitbox)){
                std::cout << "collision happening between bullet entity " << bullet << " and asteroid entity " << asteroid << std::endl;
                bool bullet_in_killqueue = false;
                bool asteroid_in_killqueue = false;

                if (std::find(entities_to_kill.begin(), entities_to_kill.end(), bullet) == entities_to_kill.end()){
                    entities_to_kill.push_back(bullet);
                }

                if (std::find(entities_to_kill.begin(), entities_to_kill.end(), asteroid) == entities_to_kill.end()){
                    entities_to_kill.push_back(asteroid);
                    game.asteroidSplit(asteroid);
                    game.randomSoundPitch("asteroid_explode");
                    game.playSound("asteroid_explode");
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

void TransformSystem::asteroidRotation(Registry& reg, float dt) {
    std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();
    std::unordered_map<Entity, AsteroidTag>& asteroidTags = reg.getComponent<AsteroidTag>();

    for (auto& [asteroid, _] : asteroidTags) {
        transforms[asteroid].rotation += sf::degrees(asteroidTags[asteroid].rotation_speed * dt);
    }
}


void TransformSystem::asteroidScreenWrap(Registry& reg, sf::Vector2u window_size) {
    std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();
    std::unordered_map<Entity, AsteroidTag>& asteroidTags = reg.getComponent<AsteroidTag>();

    for (const auto& [asteroid, __] : asteroidTags) {
        if (transforms.contains(asteroid) && asteroidTags.contains(asteroid)) {
            if (transforms[asteroid].position.x < 0 - transforms[asteroid].size_x) {
                transforms[asteroid].position.x = static_cast<float>(window_size.x) + transforms[asteroid].size_x;
            } else if (transforms[asteroid].position.x > window_size.x + transforms[asteroid].size_x) {
                transforms[asteroid].position.x = 0.f - transforms[asteroid].size_x;
            }

            if (transforms[asteroid].position.y < 0 - transforms[asteroid].size_y) {
                transforms[asteroid].position.y = static_cast<float>(window_size.y) + transforms[asteroid].size_y;
            } else if (transforms[asteroid].position.y > window_size.y + transforms[asteroid].size_y) {
                transforms[asteroid].position.y = 0.f - transforms[asteroid].size_y;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
//MOVEMENT SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

void MovementSystem::update_player(Registry& reg, float dt) {
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

////////////////////////////////////////////////////////////////////////////////////
//INPUT SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

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
                        float velocity_x = (cos(transforms[e].rotation.asRadians()) * playerTags[e].bulletSpeed) + transforms[e].velocity_x; // Adjust bullet speed x
                        float velocity_y = (sin(transforms[e].rotation.asRadians()) * playerTags[e].bulletSpeed) + transforms[e].velocity_y; // Adjust bullet speed y
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
    reg.getComponent<PlayerTag>()[player] = PlayerTag{};

    reg.getComponent<Sprite>()[player].sprite.setScale({2.f, 2.f});

    reg.getComponent<CollisionBox>()[player].collision_shape.setPosition(sf::Vector2f(400.f, 400.f));
    reg.getComponent<CollisionBox>()[player].collision_shape.setFillColor(sf::Color::Transparent);
    reg.getComponent<CollisionBox>()[player].collision_shape.setOutlineColor(sf::Color::Red);
    reg.getComponent<CollisionBox>()[player].collision_shape.setOutlineThickness(2.f);
}


void SpawnSystem::createAsteroid(Registry& reg, uint32_t size, float vx, float vy, sf::Vector2f position) {
    

    Entity asteroid = reg.create();

    float random_rotation_speed = utils::randFloat(-50.f, 50.f); // Random rotation speed between -50 and 50 degrees per second
    std::cout << "Random rotation speed for asteroid: " << random_rotation_speed << " degrees/second" << std::endl;
    reg.getComponent<AsteroidTag>()[asteroid] = AsteroidTag{size, 100.f, random_rotation_speed};
    AsteroidTag& tag = reg.getComponent<AsteroidTag>()[asteroid];

    reg.getComponent<Transform>()[asteroid] = Transform {
        sf::degrees(0.0f), 
        vx, 
        vy, 
        position, 
        200.f, 
        200.f, 
        tag.size_to_pixels[tag.size], 
        tag.size_to_pixels[tag.size]
    };

    sf::Texture asteroid_texture = sf::Texture(tag.size_to_texture[tag.size], false, sf::IntRect());
    reg.getComponent<Sprite>()[asteroid] = Sprite{.texture = asteroid_texture};
    reg.getComponent<Sprite>()[asteroid].sprite.setScale({2.f, 2.f});

    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setPosition(position);
    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setRadius(reg.getComponent<Transform>()[asteroid].size_x / 2.f); // Set radius to match the size of the asteroid sprite
    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setFillColor(sf::Color::Transparent);
    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setOutlineColor(sf::Color::Green);
    reg.getComponent<CollisionBox>()[asteroid].collision_shape.setOutlineThickness(2.f);

}


void SpawnSystem::createCursor(Registry& reg) {
    Entity cursor = reg.create();
    reg.getComponent<CursorTag>()[cursor] = CursorTag{sf::Mouse::getPosition()};
}


void SpawnSystem::createBullet(Game& game, Registry& reg, sf::Angle angle, float vx, float vy, sf::Vector2f position) {
        Entity bullet = reg.create();
        
        reg.getComponent<Sprite>()[bullet] = Sprite{};
        reg.getComponent<BulletTag>()[bullet] = BulletTag{};
        reg.getComponent<CollisionBox>()[bullet] = CollisionBox{};
        reg.getComponent<Transform>()[bullet] = Transform{angle, vx, vy, position, 5000.f, 800.f, 8.f, 8.f};

        //std::unordered_map<Entity, Sprite>& sprite = reg.getComponent<Sprite>()[bullet];
        reg.getComponent<Sprite>()[bullet].texture.loadFromFile("media/sprites/bullet.png");
        reg.getComponent<Sprite>()[bullet].sprite.setTexture(reg.getComponent<Sprite>()[bullet].texture);


        reg.getComponent<CollisionBox>()[bullet].collision_shape.setPosition(position);
        reg.getComponent<CollisionBox>()[bullet].collision_shape.setFillColor(sf::Color::Transparent);
        reg.getComponent<CollisionBox>()[bullet].collision_shape.setOutlineColor(sf::Color::Blue);
        reg.getComponent<CollisionBox>()[bullet].collision_shape.setOutlineThickness(2.f);

        game.randomSoundPitch("shoot");
        game.playSound("shoot");
}


void SpawnSystem::asteroidSplit(Registry& reg, Entity asteroid) {
    std::unordered_map<Entity, AsteroidTag>& asteroidTags = reg.getComponent<AsteroidTag>();
    std::unordered_map<Entity, Transform>& transforms = reg.getComponent<Transform>();
    const auto& tag = asteroidTags[asteroid];

        if (tag.size == 0) { // Large asteroid
            for (int i = 0; i < 2; i++) {
                float new_vx = transforms[asteroid].velocity_x + utils::randFloat(-100.f, 100.f);
                float new_vy = transforms[asteroid].velocity_y + utils::randFloat(-100.f, 100.f);
                sf::Vector2f new_position = transforms[asteroid].position + sf::Vector2f(utils::randFloat(-10.f, 10.f), utils::randFloat(-10.f, 10.f));
                this->createAsteroid(reg, 1, new_vx, new_vy, new_position);
            }
        } else if (tag.size == 1) { // Medium asteroid
            for (int i = 0; i < 2; i++) {
                float new_vx = transforms[asteroid].velocity_x + utils::randFloat(-100.f, 100.f);
                float new_vy = transforms[asteroid].velocity_y + utils::randFloat(-100.f, 100.f);
                sf::Vector2f new_position = transforms[asteroid].position + sf::Vector2f(utils::randFloat(-10.f, 10.f), utils::randFloat(-10.f, 10.f));
                this->createAsteroid(reg, 2, new_vx, new_vy, new_position);
            }
        }
}

////////////////////////////////////////////////////////////////////////////////////
//ROUND SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

void RoundSystem::newRound(Registry& reg, sf::RenderWindow& window, Game& game, uint32_t& maxAsteroids) {
    std::unordered_map<Entity, AsteroidTag>& asteroids = reg.getComponent<AsteroidTag>();

    if (game.isRoundOver()) {

        sf::Vector2u window_size = window.getSize();
        sf::Vector2f spawn_coords = utils::randBordSpawnCoord(window_size, 64.f);
        sf::Vector2f direction_to_center = sf::Vector2f(window_size.x/2, window_size.y/2) - spawn_coords;
        float angle_to_center = atan2(direction_to_center.y, direction_to_center.x);
        float angle_degrees = angle_to_center * 180 / 3.14159f; // Convert to degrees
        float random_offset = utils::randFloat(-30.f, 30.f); // Random offset in degrees

        float speed = 100.f;
        float vx = cos(angle_to_center) * speed;
        float vy = sin(angle_to_center) * speed;
        while (asteroids.size() < maxAsteroids) {
            
            spawn_coords = utils::randBordSpawnCoord(window_size, 64.f);
            utils::AsteroidSpawnParams spawnParams = utils::calculateAsteroidSpawnParams(spawn_coords, window_size, speed);

            game.createAsteroid(0, spawnParams.vx, spawnParams.vy, spawn_coords);
        }
        game.setRoundOver(false);

    }

    if (asteroids.empty()) {
        game.setRoundOver(true);
        maxAsteroids = (maxAsteroids * 2) + 1; // Increase the number of asteroids for the next round
    }

}


////////////////////////////////////////////////////////////////////////////////////
//UI SYSTEMS
////////////////////////////////////////////////////////////////////////////////////

