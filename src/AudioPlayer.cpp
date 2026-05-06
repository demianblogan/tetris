#include "AudioPlayer.h"

AudioPlayer::AudioPlayer(SoundBufferManager& soundBuffers)
	: soundBuffers(soundBuffers)
{
	// No code
}

void AudioPlayer::Play(Assets::SoundID soundID)
{
	activeSounds.emplace_back(soundID, soundBuffers.Get(soundID));
	activeSounds.back().sound.play();
}

void AudioPlayer::Restart(Assets::SoundID soundID)
{
	for (ActiveSound& activeSound : activeSounds)
	{
		if (activeSound.id == soundID)
		{
			activeSound.sound.stop();
			activeSound.sound.play();
			return;
		}
	}

	Play(soundID);
}

void AudioPlayer::RemoveStoppedSounds()
{
	activeSounds.erase(
		std::remove_if(
			activeSounds.begin(),
			activeSounds.end(),
			[](const ActiveSound& activeSound)
			{
				return activeSound.sound.getStatus() == sf::Sound::Status::Stopped;
			}),
		activeSounds.end()
	);
}