#pragma once

#include <vector>
#include <SFML/Audio/Sound.hpp>
#include "../resources/ResourceManagers.h"

struct ActiveSound
{
	Assets::SoundID id;
	sf::Sound sound;

	ActiveSound(Assets::SoundID id, const sf::SoundBuffer& buffer)
		: id(id), sound(buffer)
	{
		// No code
	}
};

class AudioPlayer
{
private:
	SoundBufferManager& soundBuffers;
	std::vector<ActiveSound> activeSounds;
	float globalVolume = 100.f;

public:
	AudioPlayer(SoundBufferManager& soundBuffers);

	void Play(Assets::SoundID soundID);
	void Restart(Assets::SoundID soundID);
	void RemoveStoppedSounds();
	void SetGlobalVolume(float volume);
};