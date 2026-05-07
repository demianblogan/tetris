#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../resources/ResourceManagers.h"

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
    SettingsManager& settings;
    AudioPlayer& audioPlayer;

    Context(
        StateMachine& stateMachine,
        sf::RenderWindow& window,
        FontManager& fonts,
        MusicManager& music,
        SoundBufferManager& soundBuffers,
        AudioPlayer& audioPlayer,
        SettingsManager& settings
    );
};