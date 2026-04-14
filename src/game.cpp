#include "ecs_simple/Registry.hpp"
#include "ecs_simple/Components.hpp"
#include "ecs_simple/Entity.hpp"
#include "ecs_simple/Systems.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "game.hpp"
#include <iostream>

/*Game::Game() {
    soundManager.loadSound("shoot", "media/sounds/pistol_shot.ogg");
    soundManager.loadSound("asteroid_explode", "media/sounds/asteroid_explosion.ogg");
}*/
 
void Game::update(sf::RenderWindow& window) {
    static int asteroids_this_round = 0;

    inputSystem.update(registry, window);
    movementSystem.update_player(registry, dt);
    
    aimSystem.update(registry, dt);
    shootingSystem.update(registry, *this, dt);
    
    //spawnSystem.newRound(registry, window, *this, asteroids_this_round);
    roundSystem.newRound(registry, window, *this, statsManager.asteroids_this_round);
    collisionSystem.update_hitbox(registry);
    collisionSystem.update_bulletcollisions(registry, *this);
    collisionSystem.update_playercollisions(registry, *this);
    
    
    transformSystem.update(registry, dt);
    transformSystem.ScreenWrap(registry, window.getSize());
    transformSystem.asteroidRotation(registry, dt);

    spriteSystem.update(registry);
}

void Game::render(sf::RenderWindow& window) {
    window.clear();
    spriteSystem.render(registry, window);
    //collisionSystem.render(registry, window);
    uiManager.renderTexts(window);
    window.display();
}

void Game::destroy(Entity entity) {
    // Clean up resources if needed
    registry.destroy(entity);
}

void Game::updateDeltaTime(float deltaTime) {
    dt = deltaTime;
}

void Game::createPlayer() {
    spawnSystem.createPlayer(registry);
}

void Game::createAsteroid(uint32_t size, float vx, float vy, sf::Vector2f position) {
    spawnSystem.createAsteroid(registry, size, vx, vy, position);
}

void Game::asteroidSplit(Entity asteroid) {
    spawnSystem.asteroidSplit(registry, asteroid);
}

void Game::createCursor() {
    spawnSystem.createCursor(registry);
}

void Game::createBullet(sf::Angle angle, float vx, float vy, sf::Vector2f position) {
    spawnSystem.createBullet(*this, registry, angle, vx, vy, position);
}

bool Game::isRoundOver() {
    return round_over;
}

void Game::setRoundOver(bool value) {
    round_over = value;
}

void Game::playSound(const std::string& soundFile) {
    soundManager.playSound(soundFile);
}

void Game::randomSoundPitch(const std::string& soundFile) {
    soundManager.randomSoundPitch(soundFile);
}

sf::SoundBuffer Game::getSoundBuffer(const std::string& file) {
    return soundManager.sound_buffers.at(file);
}

Registry& Game::getRegistry() {
    return registry;
}

void Game::updateStat(const std::string stat_name, std::string message, int amount) {
    statsManager.updateStat(stat_name, 1);
    int current_stat = statsManager.getStat(stat_name);
    uiManager.updateTextString(stat_name, message + std::to_string(current_stat));
}