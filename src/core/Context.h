#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../resources/ResourceManager.h"

class AudioPlayer;
class StateMachine;
class SettingsManager;
class HighScoreManager;

struct Context
{
    StateMachine& stateMachine;

    sf::RenderWindow& window;

    FontManager& fonts;
    MusicManager& music;
    SoundBufferManager& soundBuffers;
    TextureManager& textures;
    SettingsManager& settings;
    HighScoreManager& highScores;
    AudioPlayer& audioPlayer;

    Context(
        StateMachine& stateMachine,
        sf::RenderWindow& window,
        FontManager& fonts,
        MusicManager& music,
        SoundBufferManager& soundBuffers,
        TextureManager& textures,
        AudioPlayer& audioPlayer,
        SettingsManager& settings,
        HighScoreManager& highScores
    );
};