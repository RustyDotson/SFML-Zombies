#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>


////////////////////////////////////////////////////////////////////////////////////
//STATS MANAGER
////////////////////////////////////////////////////////////////////////////////////
struct StatsManager {
    int score = 0;
    int asteroids_spawned_this_round = 0;
    int max_asteroids_this_round = 1;
    int asteroids_remaining = 0;
    int round = 0;

    std::unordered_map<std::string, int*> stats = {
        {"score", &score},
        {"asteroids_spawned_this_round", &asteroids_spawned_this_round},
        {"max_asteroids_this_round", &max_asteroids_this_round},
        {"asteroids_remaining", &asteroids_remaining},
        {"round", &round}
    };

    void updateStat(const std::string& name, int value);
    int getStat(const std::string& name);
};


////////////////////////////////////////////////////////////////////////////////////
//SOUND MANAGER
////////////////////////////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////////////////////////////
//UI MANAGER
////////////////////////////////////////////////////////////////////////////////////
struct UIManager {
    std::unordered_map<std::string, sf::Font> fonts = {
        {"default", sf::Font("media/fonts/Samba.ttf")}
    };

    std::unordered_map<std::string, sf::Text> texts = {
        {"score", sf::Text(fonts["default"], "Score: 0", 48)},
        {"round", sf::Text(fonts["default"], "Round: 0", 48)}
    };

    int UI_text_offset;

    /*UIManager() {
        // Load default font
        sf::Font default_font;
        if (!default_font.loadFromFile("media/fonts/arial.ttf")) {
            std::cout << "Failed to load default font!" << std::endl;
        } else {
            fonts["default"] = default_font;
        }
    }*/

    //void loadFont(const std::string& name, const std::string& filePath);
    //void createText(const std::string& name, const std::string& fontName, const std::string& string, unsigned int characterSize, sf::Color color);
    UIManager();
    void updateTextString(const std::string& name, const std::string& newString);
    //void updateTextPosition(const std::string& name, sf::Vector2f newPosition);
    void renderTexts(sf::RenderWindow& window);
};