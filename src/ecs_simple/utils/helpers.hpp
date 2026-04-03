#include <SFML/Graphics.hpp>


namespace utils {
    sf::Vector2f rand_bord_spawn_coord(sf::Vector2u& window_size, float border_offset); //returns random border coordinates with an offset from the edge to prevent immediate collisions
    float rand_float(float min, float max);
}
