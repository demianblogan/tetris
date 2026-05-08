#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include "../audio/AudioPlayer.h"
#include "../resources/ResourceManager.h"
#include "../settings/SettingsManager.h"
#include "Context.h"
#include "StateMachine.h"

class Game
{
private:
	static constexpr sf::Vector2f VIRTUAL_RESOLUTION{ 1920.f, 1080.f };

	sf::RenderWindow window;
	sf::View gameView;

	StateMachine stateMachine;

	FontManager fonts;
	MusicManager music;
	SoundBufferManager soundBuffers;
	TextureManager textures;
	SettingsManager settings;
	AudioPlayer audioPlayer;

	Context context;

	bool IsWindowOpen() const;
	void ProcessEvents();
	void Update(float deltaTime);
	void Render();

public:
	Game();
	void Run();
};