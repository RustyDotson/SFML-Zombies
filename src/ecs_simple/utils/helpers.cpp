#include "helpers.hpp"
#include <random>
#include <SFML/Graphics.hpp>

namespace utils {

    sf::Vector2f rand_bord_coord(sf::Vector2u& window_size) { //returns random border coordinates

        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> sides(0, 3);
        std::uniform_real_distribution<float> x_coord(0.f, static_cast<float>(window_size.x));
        std::uniform_real_distribution<float> y_coord(0.f, static_cast<float>(window_size.y));

        int nsew = sides(rng); // 0 = north, 1 = south, 2 = east, 3 = west
        float asteroid_x = x_coord(rng);
        float asteroid_y = y_coord(rng);

        switch (nsew) {
            case 0: 
                asteroid_y = 0.f; break; // North
            case 1: 
                asteroid_y = static_cast<float>(window_size.y); break; // South
            case 2: 
                asteroid_x = static_cast<float>(window_size.x); break; // East
            case 3: 
                asteroid_x = 0.f; break; // West
        }
        
        return sf::Vector2f(asteroid_x, asteroid_y);

    }

    float rand_float(float min, float max) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }
}
