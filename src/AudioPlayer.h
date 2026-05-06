#pragma once

#include <vector>

#include <SFML/Audio/Sound.hpp>

#include "ResourceManagers.h"

struct ActiveSound
{
	Assets::SoundID id;
	sf::Sound sound;

	ActiveSound(Assets::SoundID id, const sf::SoundBuffer& buffer)
		:id(id), sound(buffer)
	{
		// No code
	}
};

class AudioPlayer
{
private:
	SoundBufferManager& soundBuffers;
	std::vector<ActiveSound> activeSounds;

public:
	AudioPlayer(SoundBufferManager& soundBuffers);

	void Play(Assets::SoundID soundID);
	void Restart(Assets::SoundID soundID);
	void RemoveStoppedSounds();
};