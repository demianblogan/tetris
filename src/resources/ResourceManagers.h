#pragma once

#include "ResourceManager.h"

using FontManager = ResourceManager<sf::Font, Assets::FontID>;
using MusicManager = ResourceManager<sf::Music, Assets::MusicID>;
using SoundBufferManager = ResourceManager<sf::SoundBuffer, Assets::SoundID>;