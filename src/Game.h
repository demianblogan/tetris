#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "AudioPlayer.h"
#include "Context.h"
#include "ResourceManager.h"
#include "State.h"

class Game
{
private:
	static constexpr sf::Vector2f VIRTUAL_RESOLUTION{ 1920.f, 1080.f };

	sf::RenderWindow window;
	sf::View gameView;

	std::unique_ptr<State> currentState;

	FontManager fonts;
	MusicManager music;
	SoundBufferManager soundBuffers;

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