#include <SFML/Audio.hpp>
#include <iostream>
#include "Managers.hpp"
#include "utils/helpers.hpp"


////////////////////////////////////////////////////////////////////////////////////
//STATS MANAGER
////////////////////////////////////////////////////////////////////////////////////

void StatsManager::updateStat(const std::string& name, int value) {
    *(stats[name]) = value;
}

int StatsManager::getStat(const std::string& name) {
    auto stat = stats.find(name);
    if (stat != stats.end()) {
        return *(stat->second);
    }
    return 0;
}



////////////////////////////////////////////////////////////////////////////////////
//SOUND MANAGER
////////////////////////////////////////////////////////////////////////////////////
void SoundManager::playSound(const std::string& name) {
    auto sound = sounds.find(name);
    if (sound != sounds.end()) {
        sound->second.play();
    } else {
        std::cout << "Sound not found: " << name << std::endl;
    }
}

void SoundManager::randomSoundPitch(const std::string& name) {
    auto sound = sounds.find(name);
    if (sound != sounds.end()) {
        sound->second.setPitch(utils::randFloat(0.25f, 1.75f));
    }
    else {
        std::cout << "unable to find sound: " << name << std::endl;
    }

    //auto sound = sounds.at(name);
    //float random_pitch = utils::randFloat(0.25f, 1.75f);
    //sound.setPitch(random_pitch);
}


////////////////////////////////////////////////////////////////////////////////////
//UI MANAGER
////////////////////////////////////////////////////////////////////////////////////
/*UIManager::UIManager() {
        // Load default font
        sf::Font default_font;
        if (!default_font.openFromFile("media/fonts/Samba.ttf")) {
            std::cout << "Failed to load default font!" << std::endl;
        } else {
            fonts["default"] = default_font;
        }
}*/

void UIManager::updateTextString(const std::string& name, const std::string& newString) {
    auto text = texts.find(name);
    if (text != texts.end()) {
        text->second.setString(newString);
    }
    else {
        std::cout << "unable to find stat: " << name << std::endl;
    }
}

void UIManager::renderTexts(sf::RenderWindow& window) {
    for (const auto [text_name, sf_text] : texts) {
        window.draw(sf_text);
    }
}