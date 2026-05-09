#include "SettingsManager.h"

#include <fstream>
#include "../audio/AudioPlayer.h"
#include "../core/Context.h"

SettingsManager::SettingsManager(const std::filesystem::path& filepath)
	: filepath(filepath)
{
    // No code
}

void SettingsManager::Load()
{
    std::ifstream file(filepath);

    if (!file.is_open())
    {
        Save();
        return;
    }

    int blockRenderStyle = 0;

    file >> settings.verticalSyncEnabled;
    file >> settings.frameRateLimit;
    file >> blockRenderStyle;
    file >> settings.soundVolume;
    file >> settings.musicVolume;

    settings.blockRenderStyle = static_cast<BlockRenderStyle>(blockRenderStyle);
}

void SettingsManager::Save() const
{
    std::ofstream file(filepath);

    file << settings.verticalSyncEnabled << '\n';
    file << settings.frameRateLimit << '\n';
    file << static_cast<int>(settings.blockRenderStyle) << '\n';
    file << settings.soundVolume << '\n';
    file << settings.musicVolume << '\n';
}

void SettingsManager::Apply(Context& context) const
{
	// --- Graphics settings ---

    context.window.setVerticalSyncEnabled(settings.verticalSyncEnabled);
    context.window.setFramerateLimit(settings.frameRateLimit);

	// --- Audio settings ---

    float musicVolume = settings.musicVolume * 10.f;
    context.music.ForEach([musicVolume](sf::Music& music)
        {
            music.setVolume(musicVolume);
        }
    );

    float soundVolume = settings.soundVolume * 10.f;
    context.audioPlayer.SetGlobalVolume(soundVolume);
}

GameSettings& SettingsManager::GetSettings()
{
    return settings;
}

const GameSettings& SettingsManager::GetSettings() const
{
    return settings;
}
