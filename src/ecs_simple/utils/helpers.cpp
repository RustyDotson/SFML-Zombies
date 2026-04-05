#include "helpers.hpp"
#include <random>
#include <SFML/Graphics.hpp>

namespace utils {

    sf::Vector2f randBordSpawnCoord(sf::Vector2u& window_size, float border_offset) { //returns random border coordinates

        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> sides(0, 3);
        std::uniform_real_distribution<float> x_coord(0.f, static_cast<float>(window_size.x));
        std::uniform_real_distribution<float> y_coord(0.f, static_cast<float>(window_size.y));

        int nsew = sides(rng); // 0 = north, 1 = south, 2 = east, 3 = west
        float asteroid_x = x_coord(rng);
        float asteroid_y = y_coord(rng);

        switch (nsew) {
            case 0: 
                asteroid_y = 0.f - border_offset; break; // North
            case 1: 
                asteroid_y = static_cast<float>(window_size.y) + border_offset; break; // South
            case 2: 
                asteroid_x = static_cast<float>(window_size.x) + border_offset; break; // East
            case 3: 
                asteroid_x = 0.f - border_offset; break; // West
        }
        
        return sf::Vector2f(asteroid_x, asteroid_y);

    }

    float randFloat(float min, float max) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }


    bool collisionCheck(sf::CircleShape cb1, sf::CircleShape cb2) {
        bool is_collision = false;
        sf::Vector2f cb1_coords = cb1.getPosition();
        sf::Vector2f cb2_coords = cb2.getPosition();

        float dx = cb1_coords.x - cb2_coords.x;
        float dy = cb1_coords.y - cb2_coords.y;

        float radius_total = cb1.getRadius() + cb2.getRadius();
        float distance = sqrt(pow(dx, 2) + pow(dy, 2));

        if (distance <= radius_total) {
            is_collision = true;
        }
        return is_collision;
    }

    AsteroidSpawnParams calculateAsteroidSpawnParams(sf::Vector2f spawn_coords, sf::Vector2u window_size, float speed) {
        sf::Vector2f direction_to_center = sf::Vector2f(window_size.x/2, window_size.y/2) - spawn_coords;
        float angle_to_center = atan2(direction_to_center.y, direction_to_center.x);
        float angle_degrees = angle_to_center * 180 / 3.14159f; // Convert to degrees
        float random_offset = randFloat(-30.f, 30.f); // Random offset in degrees
        
        angle_degrees += random_offset; // Add random offset to the angle
        angle_to_center = angle_degrees * 3.14159f / 180; // Convert back to radians

        float vx = cos(angle_to_center) * speed;
        float vy = sin(angle_to_center) * speed;

        return {vx, vy, angle_degrees};
    }
}
