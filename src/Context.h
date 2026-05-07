#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "AudioPlayer.h"
#include "ResourceManagers.h"

class StateMachine;

struct Context
{
    StateMachine& stateMachine;

    sf::RenderWindow& window;

    FontManager& fonts;
    MusicManager& music;
    SoundBufferManager& soundBuffers;

    AudioPlayer& audioPlayer;
};