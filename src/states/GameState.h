#pragma once

#include "../core/State.h"
#include "../core/Context.h"
#include "../game/Board.h"
#include "../game/Tetromino.h"
#include "../game/TetrominoBag.h"
#include "../ui/Label.h"
#include "../ui/Layout.h"

class GameState : public State
{
private:
	static constexpr float BLOCK_SIZE = 48.f;
	static constexpr int WALL_TEXTURE_INDEX = 10;
	static constexpr sf::Vector2f BOARD_POSITION{ 720.f, 60.f };
	static constexpr int SCORE_PER_LEVEL = 50;

	Context& context;

	Board board;
	TetrominoBag tetrominoBag;
	Tetromino currentTetromino;
	Tetromino nextTetromino;

	float fallTimer = 0.f;
	float fallDelay = 0.5f;

	int score = 0;
	std::unique_ptr<UI::Layout> hudLayout;

	UI::Label* nextTetrominoLabel = nullptr;
	UI::Label* scoreLabel = nullptr;
	UI::Label* levelLabel = nullptr;

	int level = 1;

	bool SpawnTetromino();
	void TryMoveTetromino(int offsetX, int offsetY);
	void TryRotateTetromino();
	void TryDropTetromino();
	void HandleTetrominoLanding();

	[[nodiscard]] Assets::TextureID GetBlockTextureID() const;

public:
	explicit GameState(Context& context);

	void ProcessEvents(sf::RenderWindow& window) override;
	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& window) override;
};