#include "Game.h"

#include <states/MainMenuState.h>

bool Game::IsWindowOpen() const
{
	return window.isOpen();
}

void Game::ProcessEvents()
{
	if (State* currentState = stateMachine.GetCurrentState())
	{
		currentState->ProcessEvents(window);
	}
}

void Game::Update(float deltaTime)
{
	if (State* currentState = stateMachine.GetCurrentState())
	{
		currentState->Update(deltaTime);
	}
}

void Game::Render()
{
	window.clear();

	window.setView(gameView);

	stateMachine.RenderStates(window);

	window.display();
}

Game::Game()
	: window(sf::VideoMode::getDesktopMode(), "Tetris", sf::State::Fullscreen)
	, gameView({ VIRTUAL_RESOLUTION / 2.f, VIRTUAL_RESOLUTION })
	, audioPlayer(soundBuffers)
	, settings(Data::Paths::Settings)
	, context(stateMachine, window, fonts, music, soundBuffers, textures, audioPlayer, settings, highScores)
	, highScores(Data::Paths::Scores)
{
	window.setMouseCursorVisible(false);
	window.setView(gameView);

	fonts.Load(Assets::FontID::Main, Assets::Paths::Fonts::Main);

	textures.Load(Assets::TextureID::BlockSpritesheetWithOutline, Assets::Paths::Textures::BlockSpritesheetWithOutline);
	textures.Load(Assets::TextureID::BlockSpritesheetWithoutOutline, Assets::Paths::Textures::BlockSpritesheetWithoutOutline);
	textures.Load(Assets::TextureID::ButtonBackground, Assets::Paths::Textures::ButtonBackground);
	textures.Load(Assets::TextureID::MenuBackground, Assets::Paths::Textures::MenuBackground);
	textures.Load(Assets::TextureID::TitleBackground, Assets::Paths::Textures::TitleBackground);
	textures.Load(Assets::TextureID::PanelBackground, Assets::Paths::Textures::PanelBackground);
	textures.Load(Assets::TextureID::GameBackground, Assets::Paths::Textures::GameBackground);

	music.Load(Assets::MusicID::MainMenu, Assets::Paths::Music::MainMenu);
	music.Load(Assets::MusicID::Gameplay, Assets::Paths::Music::Gameplay);
	music.Load(Assets::MusicID::GameOver, Assets::Paths::Music::GameOver);

	soundBuffers.Load(Assets::SoundID::MenuItemSelected, Assets::Paths::Sounds::MenuItemSelected);
	soundBuffers.Load(Assets::SoundID::MenuItemPressed, Assets::Paths::Sounds::MenuItemPressed);
	soundBuffers.Load(Assets::SoundID::DropPiece, Assets::Paths::Sounds::DropPiece);
	soundBuffers.Load(Assets::SoundID::MovePiece, Assets::Paths::Sounds::MovePiece);
	soundBuffers.Load(Assets::SoundID::RotatePiece, Assets::Paths::Sounds::RotatePiece);
	soundBuffers.Load(Assets::SoundID::PieceHitWall, Assets::Paths::Sounds::PieceHitWall);
	soundBuffers.Load(Assets::SoundID::NextLevel, Assets::Paths::Sounds::NextLevel);
	soundBuffers.Load(Assets::SoundID::RowCleared, Assets::Paths::Sounds::RowCleared);

	settings.Load();
	settings.Apply(context);

	highScores.Load();

	stateMachine.PushState(std::make_unique<MainMenuState>(context));
}

void Game::Run()
{
	sf::Clock deltaTimeClock;

	while (IsWindowOpen())
	{
		float deltaTime = deltaTimeClock.restart().asSeconds();

		ProcessEvents();
		stateMachine.ApplyPendingChanges();

		Update(deltaTime);
		stateMachine.ApplyPendingChanges();

		Render();

		audioPlayer.RemoveStoppedSounds();
	}
}