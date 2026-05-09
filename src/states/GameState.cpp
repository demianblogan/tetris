// GameState.cpp

#include "GameState.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "../resources/Assets.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include "../core/Context.h"
#include "../core/StateMachine.h"
#include "MainMenuState.h"
#include <settings/GameSettings.h>
#include "../settings/SettingsManager.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "../audio/AudioPlayer.h"

GameState::GameState(Context& context)
	: context(context)
	, currentTetromino(tetrominoBag.Next(), { Board::WIDTH / 2 - 2, 0 })
	, nextTetromino(tetrominoBag.Next(), { 0, 0 })
	, backgroundSprite(context.textures.Get(Assets::TextureID::GameBackground))
{
	backgroundSprite.setColor(sf::Color(150, 150, 150));

	rightHudLayout = std::make_unique<UI::Layout>(UI::Layout::Orientation::Vertical);
	rightHudLayout->SetGap(32.f);

	sf::Sprite panelSprite(context.textures.Get(Assets::TextureID::PanelBackground));

	// =====================================================
	// Next tetromino panel
	// =====================================================
	{
		auto panel = std::make_unique<UI::Panel>(panelSprite);

		auto layout = std::make_unique<UI::Layout>(UI::Layout::Orientation::Vertical);
		layout->SetGap(20.f);

		auto label = std::make_unique<UI::Label>(context.fonts.Get(Assets::FontID::Main), "Next Tetromino", 60);
		label->SetFillColor(sf::Color::White);
		nextTetrominoLabel = label.get();
		layout->Add(std::move(label));

		panel->SetChild(std::move(layout));
		panel->SetWidthPixels(450.f);
		panel->SetHeightPixels(260.f);
		panel->SetPadding({ 60.f, 50.f });

		rightHudLayout->Add(std::move(panel));
	}

	// =====================================================
	// Score panel
	// =====================================================
	{
		auto panel = std::make_unique<UI::Panel>(panelSprite);

		auto layout = std::make_unique<UI::Layout>(UI::Layout::Orientation::Vertical);
		layout->SetGap(30.f);

		// =================================================
		// Score label
		// =================================================
		{
			auto label = std::make_unique<UI::Label>(context.fonts.Get(Assets::FontID::Main), "Score: 0", 60);
			label->SetFillColor(sf::Color::White);
			scoreLabel = label.get();
			layout->Add(std::move(label));
		}

		// =================================================
		// Level label
		// =================================================
		{
			auto label = std::make_unique<UI::Label>(context.fonts.Get(Assets::FontID::Main), "Level: 1", 60);
			label->SetFillColor(sf::Color::White);
			levelLabel = label.get();
			layout->Add(std::move(label));
		}

		panel->SetChild(std::move(layout));
		panel->SetWidthPixels(340.f);
		panel->SetHeightPixels(200.f);
		panel->SetPadding({ 60.f, 50.f });

		rightHudLayout->Add(std::move(panel));
	}

	// =====================================================
	// Controls panel
	// =====================================================

	sf::Sprite controlsSprite(context.textures.Get(Assets::TextureID::PanelBackground));

	controlsPanel = std::make_unique<UI::Panel>(controlsSprite);

	auto controlsLayout = std::make_unique<UI::Layout>(UI::Layout::Orientation::Vertical);

	controlsLayout->SetPadding(
		{
			.left = 80.f,
			.top = 50.f,
		}
	);

	sf::String controlsText =
		L"[←] [↓] [→] - Move tetromino\n"
		L"[↑] - Rotate tetromino\n"
		L"[Space] - Drop tetromino\n"
		L"[ESC] - Pause";

	auto controlsLabel = std::make_unique<UI::Label>(context.fonts.Get(Assets::FontID::Main), controlsText, 45);
	controlsLabel->SetFillColor(sf::Color::White);
	controlsLayout->Add(std::move(controlsLabel));

	controlsPanel->SetChild(std::move(controlsLayout));
	controlsPanel->SetWidthPixels(620.f);
	controlsPanel->SetHeightPixels(300.f);

	const sf::Vector2f rightHudSize = rightHudLayout->Measure();

	rightHudLayout->Arrange(
		{
			BOARD_POSITION.x + Board::WIDTH * BLOCK_SIZE + 100.f,
			BOARD_POSITION.y
		},
		rightHudSize
	);

	controlsPanel->Arrange(
		{
			10,
			BOARD_POSITION.y
		},
		{ 620.f, 300.f }
	);

	nextTetrominoPreviewPosition =
	{
		BOARD_POSITION.x + Board::WIDTH * BLOCK_SIZE + 240.f,
		BOARD_POSITION.y + 120.f
	};

	context.music.Get(Assets::MusicID::MainMenu).stop();

	sf::Music& music = context.music.Get(Assets::MusicID::Gameplay);
	music.setLooping(true);

	if (music.getStatus() != sf::Music::Status::Playing)
	{
		music.play();
	}
}

void GameState::ProcessEvents(sf::RenderWindow& window)
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			window.close();
		}

		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			switch (keyPressed->scancode)
			{
			case sf::Keyboard::Scancode::Left:
				TryMoveTetromino(-1, 0);
				break;

			case sf::Keyboard::Scancode::Right:
				TryMoveTetromino(1, 0);
				break;

			case sf::Keyboard::Scancode::Down:
				TryMoveTetromino(0, 1);
				break;

			case sf::Keyboard::Scancode::Up:
				TryRotateTetromino();
				break;

			case sf::Keyboard::Scancode::Space:
				TryDropTetromino();
				break;

			case sf::Keyboard::Scancode::Escape:
				context.stateMachine.PushState(std::make_unique<PauseState>(context));
				break;
			}
		}
	}
}

void GameState::Update(float deltaTime)
{
	fallTimer += deltaTime;

	if (fallTimer >= fallDelay)
	{
		fallTimer -= fallDelay;

		Tetromino movedTetromino = currentTetromino;

		movedTetromino.Move(0, 1);

		if (board.CanPlace(movedTetromino))
		{
			currentTetromino = movedTetromino;
		}
		else
		{
			HandleTetrominoLanding();
		}
	}
}

void GameState::Render(sf::RenderWindow& window)
{
	window.draw(backgroundSprite);

	// =====================================================
	// Render board background tiles
	// =====================================================

	constexpr int SPRITE_SIZE = 16;

	sf::Sprite blockSprite(context.textures.Get(GetBlockTextureID()));

	blockSprite.setScale(
		{
			BLOCK_SIZE / 16.f,
			BLOCK_SIZE / 16.f
		}
	);

	blockSprite.setTextureRect(
		{
			{ WALL_TEXTURE_INDEX * SPRITE_SIZE, 0 },
			{ SPRITE_SIZE, SPRITE_SIZE }
		}
	);

	for (int y = 0; y < Board::HEIGHT; y++)
	{
		// =================================================
		// Gradient
		// =================================================

		const float t =
			static_cast<float>(y)
			/ (Board::HEIGHT - 1);

		const int  brightness = static_cast<int>(6 + t * 18);

		// Холодный sci-fi оттенок
		blockSprite.setColor(
			sf::Color(
				brightness / 2,
				brightness,
				brightness + 20
			)
		);

		for (int x = 0; x < Board::WIDTH; x++)
		{
			blockSprite.setPosition(
				{
					BOARD_POSITION.x + x * BLOCK_SIZE,
					BOARD_POSITION.y + y * BLOCK_SIZE
				}
			);

			window.draw(blockSprite);
		}
	}

	blockSprite.setColor(sf::Color::White);

	// =====================================================
	// Render walls
	// =====================================================

	blockSprite.setTextureRect(
		{
			{ WALL_TEXTURE_INDEX * SPRITE_SIZE, 0 },
			{ SPRITE_SIZE, SPRITE_SIZE }
		}
	);

	for (int y = 0; y < Board::HEIGHT; y++)
	{
		// Left wall
		blockSprite.setPosition(
			{
				BOARD_POSITION.x - BLOCK_SIZE,
				BOARD_POSITION.y + y * BLOCK_SIZE
			}
		);

		window.draw(blockSprite);

		// Right wall
		blockSprite.setPosition(
			{
				BOARD_POSITION.x + Board::WIDTH * BLOCK_SIZE,
				BOARD_POSITION.y + y * BLOCK_SIZE
			}
		);

		window.draw(blockSprite);
	}

	// Bottom wall
	for (int x = -1; x <= Board::WIDTH; x++)
	{
		blockSprite.setPosition(
			{
				BOARD_POSITION.x + x * BLOCK_SIZE,
				BOARD_POSITION.y + Board::HEIGHT * BLOCK_SIZE
			}
		);

		window.draw(blockSprite);
	}

	// =====================================================
	// Render board
	// =====================================================

	const Board::Grid& grid = board.GetGrid();

	for (int y = 0; y < Board::HEIGHT; ++y)
	{
		for (int x = 0; x < Board::WIDTH; x++)
		{
			const Cell& cell = grid[y][x];

			if (!cell.occupied)
			{
				continue;
			}

			const int textureX =
				static_cast<int>(cell.tetrominoType)
				* SPRITE_SIZE;

			blockSprite.setTextureRect(
				{
					{ textureX, 0 },
					{ SPRITE_SIZE, SPRITE_SIZE }
				}
			);

			blockSprite.setPosition(
				{
					BOARD_POSITION.x + x * BLOCK_SIZE,
					BOARD_POSITION.y + y * BLOCK_SIZE
				}
			);

			window.draw(blockSprite);
		}
	}

	// =====================================================
	// Render ghost tetromino
	// =====================================================

	const Tetromino ghostTetromino = GetGhostTetromino();

	const auto ghostBlockPositions =
		ghostTetromino.GetBlockPositions();

	const int ghostTextureX =
		static_cast<int>(ghostTetromino.GetType())
		* SPRITE_SIZE;

	blockSprite.setTextureRect(
		{
			{ ghostTextureX, 0 },
			{ SPRITE_SIZE, SPRITE_SIZE }
		}
	);

	blockSprite.setColor(
		sf::Color(255, 255, 255, 80)
	);

	for (const sf::Vector2i& blockPosition : ghostBlockPositions)
	{
		blockSprite.setPosition(
			{
				BOARD_POSITION.x + blockPosition.x * BLOCK_SIZE,
				BOARD_POSITION.y + blockPosition.y * BLOCK_SIZE
			}
		);

		window.draw(blockSprite);
	}

	blockSprite.setColor(sf::Color::White);

	// =====================================================
	// Render current tetromino
	// =====================================================

	const auto blockPositions =
		currentTetromino.GetBlockPositions();

	const int textureX =
		static_cast<int>(currentTetromino.GetType())
		* SPRITE_SIZE;

	blockSprite.setTextureRect(
		{
			{ textureX, 0 },
			{ SPRITE_SIZE, SPRITE_SIZE }
		}
	);

	for (const sf::Vector2i& blockPosition : blockPositions)
	{
		blockSprite.setPosition(
			{
				BOARD_POSITION.x + blockPosition.x * BLOCK_SIZE,
				BOARD_POSITION.y + blockPosition.y * BLOCK_SIZE
			}
		);

		window.draw(blockSprite);
	}

	// =====================================================
	// Render UI
	// =====================================================

	rightHudLayout->Render(window);
	controlsPanel->Render(window);

	// =====================================================
	// Render next tetromino preview
	// =====================================================

	const auto previewBlockPositions =
		nextTetromino.GetBlockPositions();

	const int previewTextureX =
		static_cast<int>(nextTetromino.GetType())
		* SPRITE_SIZE;

	blockSprite.setTextureRect(
		{
			{ previewTextureX, 0 },
			{ SPRITE_SIZE, SPRITE_SIZE }
		}
	);

	constexpr float PREVIEW_BLOCK_SIZE = 36.f;

	blockSprite.setScale(
		{
			PREVIEW_BLOCK_SIZE / 16.f,
			PREVIEW_BLOCK_SIZE / 16.f
		}
	);

	for (const sf::Vector2i& blockPosition : previewBlockPositions)
	{
		blockSprite.setPosition(
			{
				nextTetrominoPreviewPosition.x + blockPosition.x * PREVIEW_BLOCK_SIZE,
				nextTetrominoPreviewPosition.y + blockPosition.y * PREVIEW_BLOCK_SIZE
			}
		);

		window.draw(blockSprite);
	}
}

bool GameState::SpawnTetromino()
{
	currentTetromino = { nextTetromino.GetType(), { Board::WIDTH / 2 - 2, 0 } };
	nextTetromino = { tetrominoBag.Next(), { 0, 0 } };

	return board.CanPlace(currentTetromino);
}

void GameState::TryMoveTetromino(int offsetX, int offsetY)
{
	Tetromino movedTetromino = currentTetromino;

	movedTetromino.Move(offsetX, offsetY);

	if (!board.CanPlace(movedTetromino))
	{
		context.audioPlayer.Play(Assets::SoundID::PieceHitWall);
		return;
	}

	currentTetromino = movedTetromino;

	context.audioPlayer.Play(Assets::SoundID::MovePiece);
}

void GameState::TryRotateTetromino()
{
	Tetromino rotatedTetromino = currentTetromino;

	rotatedTetromino.RotateClockwise();

	if (!board.CanPlace(rotatedTetromino))
	{
		context.audioPlayer.Play(Assets::SoundID::PieceHitWall);
		return;
	}

	currentTetromino = rotatedTetromino;

	context.audioPlayer.Play(Assets::SoundID::RotatePiece);
}

void GameState::TryDropTetromino()
{
	while (true)
	{
		Tetromino movedTetromino = currentTetromino;

		movedTetromino.Move(0, 1);

		if (!board.CanPlace(movedTetromino))
		{
			break;
		}

		currentTetromino = movedTetromino;
	}

	context.audioPlayer.Play(Assets::SoundID::DropPiece);

	board.LockTetromino(currentTetromino);
	HandleTetrominoLanding();

	fallTimer = 0.f;
}

void GameState::HandleTetrominoLanding()
{
	board.LockTetromino(currentTetromino);

	const int clearedRows = board.ClearFullRows();
	if (clearedRows != 0)
	{
		context.audioPlayer.Play(Assets::SoundID::RowCleared);
	}

	score += clearedRows * 10;

	const int previousLevel = level;
	level = score / SCORE_PER_LEVEL + 1;
	if (level > previousLevel)
	{
		context.audioPlayer.Play(Assets::SoundID::NextLevel);
	}

	fallDelay = std::max(0.1f, 0.5f - (level - 1) * 0.05f);

	scoreLabel->SetString("Score: " + std::to_string(score));
	levelLabel->SetString("Level: " + std::to_string(level));

	if (!SpawnTetromino())
	{
		context.stateMachine.ChangeState(std::make_unique<GameOverState>(context, score));
	}
}

Tetromino GameState::GetGhostTetromino() const
{
	Tetromino ghostTetromino = currentTetromino;

	while (true)
	{
		Tetromino movedTetromino = ghostTetromino;

		movedTetromino.Move(0, 1);

		if (!board.CanPlace(movedTetromino))
		{
			break;
		}

		ghostTetromino = movedTetromino;
	}

	return ghostTetromino;
}

Assets::TextureID GameState::GetBlockTextureID() const
{
	switch (context.settings.GetSettings().blockRenderStyle)
	{
	case BlockRenderStyle::WithOutline:
		return Assets::TextureID::BlockSpritesheetWithOutline;

	case BlockRenderStyle::WithoutOutline:
		return Assets::TextureID::BlockSpritesheetWithoutOutline;
	}

	std::unreachable();
}