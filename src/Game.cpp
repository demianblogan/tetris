#include "Game.h"

#include "MainMenuState.h"

bool Game::IsWindowOpen() const
{
	return window.isOpen();
}

void Game::ProcessEvents()
{
	currentState->ProcessEvents(window);
}

void Game::Update(float deltaTime)
{
	currentState->Update(deltaTime);
}

void Game::Render()
{
	window.clear();

	window.setView(gameView);

	currentState->Render(window);

	window.display();
}

Game::Game()
	: window(sf::VideoMode::getDesktopMode(), "Tetris", sf::State::Fullscreen)
	, gameView({ VIRTUAL_RESOLUTION / 2.f, VIRTUAL_RESOLUTION })
	, audioPlayer(soundBuffers)
	, context(window, fonts, music, soundBuffers, audioPlayer)
{
	window.setView(gameView);
	window.setVerticalSyncEnabled(true);

	fonts.Load(Assets::FontID::Main, Assets::Paths::Fonts::Main);

	music.Load(Assets::MusicID::MainMenu, Assets::Paths::Music::MainMenu);

	soundBuffers.Load(Assets::SoundID::MenuItemSelected, Assets::Paths::Sounds::MenuItemSelected);
	soundBuffers.Load(Assets::SoundID::MenuItemPressed, Assets::Paths::Sounds::MenuItemPressed);

	currentState = std::make_unique<MainMenuState>(context);
}

void Game::Run()
{
	sf::Clock deltaTimeClock;

	while (IsWindowOpen())
	{
		float deltaTime = deltaTimeClock.reset().asSeconds();

		ProcessEvents();
		Update(deltaTime);
		Render();

		audioPlayer.RemoveStoppedSounds();
	}
}