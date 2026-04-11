#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>

struct SoundManager {
    std::unordered_map<std::string, sf::SoundBuffer> sound_buffers = {
        {"shoot", sf::SoundBuffer("media/sounds/pistol_shot.ogg")},
        {"asteroid_explode", sf::SoundBuffer("media/sounds/asteroid_explosion.ogg")}
    };
    std::unordered_map<std::string, sf::Sound> sounds = {
        {"shoot", sf::Sound(sound_buffers["shoot"])},
        {"asteroid_explode", sf::Sound(sound_buffers["asteroid_explode"])}
    };

    //void loadSound(const std::string& name, const std::string& filePath);
    void playSound(const std::string& name);
    void randomSoundPitch(const std::string& name);
    
};