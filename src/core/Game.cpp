#include "Game.h"

#include <states/MainMenuState.h>
#include <states/PauseState.h>

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
	context.totalTime += deltaTime;

	if (State* currentState = stateMachine.GetCurrentState())
	{
		currentState->Update(deltaTime);
	}
}

void Game::Render()
{
	sf::Shader& crtShader = context.shaders.Get(Assets::ShaderID::CRT);
	sf::Shader& blurShader = context.shaders.Get(Assets::ShaderID::Blur);

	State* currentState = stateMachine.GetCurrentState();

	const bool isPause =
		dynamic_cast<PauseState*>(currentState) != nullptr;

	// =====================================================
	// NORMAL RENDER
	// =====================================================

	if (!isPause)
	{
		renderTexture.clear();
		renderTexture.setView(gameView);
		stateMachine.RenderStates(renderTexture);
		renderTexture.display();

		sf::Sprite screenSprite(renderTexture.getTexture());

		window.clear();
		crtShader.setUniform("time", context.totalTime);
		window.draw(screenSprite, &crtShader);
		window.display();

		return;
	}

	// =====================================================
	// Render gameplay only
	// =====================================================

	gameplayTexture.clear();
	gameplayTexture.setView(gameView);
	stateMachine.RenderStatesExceptTop(gameplayTexture);
	gameplayTexture.display();

	// =====================================================
	// Compose final scene
	// =====================================================

	finalTexture.clear();

	sf::Sprite gameplaySprite(gameplayTexture.getTexture());
	finalTexture.draw(gameplaySprite, &blurShader);
	stateMachine.RenderTopState(finalTexture);
	finalTexture.display();

	// =====================================================
	// Final CRT pass
	// =====================================================

	sf::Sprite finalSprite(finalTexture.getTexture());
	window.clear();
	crtShader.setUniform("time", context.totalTime);
	window.draw(finalSprite, &crtShader);
	window.display();
}

Game::Game()
	: window(sf::VideoMode::getDesktopMode(), "Tetris", sf::State::Fullscreen)
	, gameView({ VIRTUAL_RESOLUTION / 2.f, VIRTUAL_RESOLUTION })
	, audioPlayer(soundBuffers)
	, settings(Data::Paths::Settings)
	, context(
		stateMachine,
		window,
		fonts,
		music,
		soundBuffers,
		textures,
		shaders,
		audioPlayer,
		settings,
		highScores)
	, highScores(Data::Paths::Scores)
{
	window.setMouseCursorVisible(false);
	window.setView(gameView);

	renderTexture.resize(
		{
			static_cast<unsigned int>(VIRTUAL_RESOLUTION.x),
			static_cast<unsigned int>(VIRTUAL_RESOLUTION.y)
		}
	);

	gameplayTexture.resize(
		{
			static_cast<unsigned int>(VIRTUAL_RESOLUTION.x),
			static_cast<unsigned int>(VIRTUAL_RESOLUTION.y)
		}
	);

	finalTexture.resize(
		{
			static_cast<unsigned int>(VIRTUAL_RESOLUTION.x),
			static_cast<unsigned int>(VIRTUAL_RESOLUTION.y)
		}
	);

	shaders.Load(Assets::ShaderID::CRT, Assets::Paths::Shaders::CRT, sf::Shader::Type::Fragment);
	shaders.Load(Assets::ShaderID::Blur, Assets::Paths::Shaders::Blur, sf::Shader::Type::Fragment);
	shaders.Load(Assets::ShaderID::Glow, Assets::Paths::Shaders::Glow, sf::Shader::Type::Fragment);
	shaders.Load(Assets::ShaderID::GhostTetromino, Assets::Paths::Shaders::GhostTetromino, sf::Shader::Type::Fragment);

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