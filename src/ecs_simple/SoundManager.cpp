#include <SFML/Audio.hpp>
#include <iostream>
#include "SoundManager.hpp"

/*void SoundManager::loadSound(const std::string& name, const std::string& filePath) {
        sf::SoundBuffer buffer;
        if (buffer.loadFromFile(filePath)) {
            sound_buffers[name] = buffer;
            sf::Sound sound(sound_buffers[name]);
            sounds[name] = sound;
        } else {
            std::cout << "Error loading sound: " << filePath << std::endl;
        }
    }*/

void SoundManager::playSound(const std::string& name) {
    auto sound = sounds.find(name);
    if (sound != sounds.end()) {
        sound->second.play();
    } else {
        std::cout << "Sound not found: " << name << std::endl;
    }
}