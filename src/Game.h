#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "AudioPlayer.h"
#include "Context.h"
#include "ResourceManager.h"
#include "StateMachine.h"
#include "SettingsManager.h"

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