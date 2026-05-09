#include "Context.h"

#include "../audio/AudioPlayer.h"
#include "../settings/SettingsManager.h"

Context::Context(
    StateMachine& stateMachine,
    sf::RenderWindow& window,
    FontManager& fonts,
    MusicManager& music,
    SoundBufferManager& soundBuffers,
    TextureManager& textures,
    AudioPlayer& audioPlayer,
    SettingsManager& settings,
    HighScoreManager& highScores
)
    : stateMachine(stateMachine)
    , window(window)
    , fonts(fonts)
    , music(music)
    , soundBuffers(soundBuffers)
    , textures(textures)
    , audioPlayer(audioPlayer)
    , settings(settings)
    , highScores(highScores)
{
	// No code
}