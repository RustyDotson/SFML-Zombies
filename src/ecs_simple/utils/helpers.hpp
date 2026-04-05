#include <SFML/Graphics.hpp>
#include <math.h>
#include "../Components.hpp"


namespace utils {
    sf::Vector2f randBordSpawnCoord(sf::Vector2u& window_size, float border_offset); //returns random border coordinates with an offset from the edge to prevent immediate collisions
    float randFloat(float min, float max);
    bool collisionCheck(sf::CircleShape cb1, sf::CircleShape cb2);

    
    struct AsteroidSpawnParams {
        float vx, vy;
        float angle_degrees;
    };
    AsteroidSpawnParams calculateAsteroidSpawnParams(sf::Vector2f spawn_coords, sf::Vector2u window_size, float speed);
}
