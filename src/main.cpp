#include "ecs_simple/Entity.hpp"
#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Components.hpp"
#include "ecs_simple/Systems.hpp"
#include "game.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>



int main(){
    unsigned int windowSize = 800;
    sf::RenderWindow window(sf::VideoMode({windowSize, windowSize}), "Asteroids!");

    Game game;
    game.createPlayer();
    game.createCursor();

    sf::Clock clock;

    while (window.isOpen())
    {  
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        float dt = clock.restart().asSeconds();
        game.updateDeltaTime(dt);

        game.update();
        game.render(window);

        
    }

    
    return 0;
}