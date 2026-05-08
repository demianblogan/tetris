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

GameState::GameState(Context& context)
	: context(context)
	, currentTetromino(tetrominoBag.Next(), { Board::WIDTH / 2 - 2, 0 })
{
	// No code
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
				context.stateMachine.ChangeState(std::make_unique<MainMenuState>(context));
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
			board.LockTetromino(currentTetromino);
			board.ClearFullRows();
			SpawnTetromino();
		}
	}
}

void GameState::Render(sf::RenderWindow& window)
{
	// =====================================================
	// Render board background
	// =====================================================

	sf::RectangleShape boardBackground;

	boardBackground.setPosition(BOARD_POSITION);
	boardBackground.setSize(
		{
			Board::WIDTH * BLOCK_SIZE,
			Board::HEIGHT * BLOCK_SIZE
		}
	);
	boardBackground.setFillColor(sf::Color(0, 70, 70));

	window.draw(boardBackground);

	sf::Sprite blockSprite(context.textures.Get(GetBlockTextureID()));
	blockSprite.setScale({ BLOCK_SIZE / 16.f, BLOCK_SIZE / 16.f });

	constexpr int SPRITE_SIZE = 16;

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
				BOARD_POSITION.x
				+ Board::WIDTH * BLOCK_SIZE,

				BOARD_POSITION.y
				+ y * BLOCK_SIZE
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

			const int textureX = static_cast<int>(cell.tetrominoType) * SPRITE_SIZE;

			blockSprite.setTextureRect({ { textureX, 0 }, { SPRITE_SIZE, SPRITE_SIZE } });
			blockSprite.setPosition({ BOARD_POSITION.x + x * BLOCK_SIZE, BOARD_POSITION.y + y * BLOCK_SIZE });

			window.draw(blockSprite);
		}
	}

	// =====================================================
	// Render current tetromino
	// =====================================================

	const auto blockPositions = currentTetromino.GetBlockPositions();

	const int textureX = static_cast<int>(currentTetromino.GetType()) * SPRITE_SIZE;

	blockSprite.setTextureRect({ { textureX, 0 }, { SPRITE_SIZE, SPRITE_SIZE } });

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
}

void GameState::SpawnTetromino()
{
	currentTetromino =
	{
		tetrominoBag.Next(),
		{ Board::WIDTH / 2 - 2, 0 }
	};
}

void GameState::TryMoveTetromino(int offsetX, int offsetY)
{
	Tetromino movedTetromino = currentTetromino;

	movedTetromino.Move(offsetX, offsetY);

	if (!board.CanPlace(movedTetromino))
	{
		return;
	}

	currentTetromino = movedTetromino;
}

void GameState::TryRotateTetromino()
{
	Tetromino rotatedTetromino = currentTetromino;

	rotatedTetromino.RotateClockwise();

	if (!board.CanPlace(rotatedTetromino))
	{
		return;
	}

	currentTetromino = rotatedTetromino;
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

	board.LockTetromino(currentTetromino);
	board.ClearFullRows();

	SpawnTetromino();

	fallTimer = 0.f;
}

Assets::TextureID
GameState::GetBlockTextureID() const
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