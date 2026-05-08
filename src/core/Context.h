#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../resources/ResourceManager.h"

class AudioPlayer;
class StateMachine;
class SettingsManager;

struct Context
{
    StateMachine& stateMachine;

    sf::RenderWindow& window;

    FontManager& fonts;
    MusicManager& music;
    SoundBufferManager& soundBuffers;
    TextureManager& textures;
    SettingsManager& settings;
    AudioPlayer& audioPlayer;

    Context(
        StateMachine& stateMachine,
        sf::RenderWindow& window,
        FontManager& fonts,
        MusicManager& music,
        SoundBufferManager& soundBuffers,
        TextureManager& textures,
        AudioPlayer& audioPlayer,
        SettingsManager& settings
    );
};