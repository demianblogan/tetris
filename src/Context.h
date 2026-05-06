#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "AudioPlayer.h"
#include "ResourceManagers.h"

struct Context
{
    sf::RenderWindow& window;

    FontManager& fonts;
    MusicManager& music;
    SoundBufferManager& soundBuffers;

    AudioPlayer& audioPlayer;
};