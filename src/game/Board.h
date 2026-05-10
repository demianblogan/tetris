#pragma once

#include <array>
#include "Cell.h"
#include "Tetromino.h"
#include <vector>

class Board
{
public:
	static constexpr int WIDTH = 10;
	static constexpr int HEIGHT = 20;

	using GridRow = std::array<Cell, WIDTH>;
	using Grid = std::array<GridRow, HEIGHT>;

private:
	Grid grid;

public:
	void LockTetromino(const Tetromino& tetromino);
	int ClearFullRows();

	[[nodiscard]] bool Contains(const Tetromino& tetromino) const;
	[[nodiscard]] bool IntersectsLockedCells(const Tetromino& tetromino) const;
	[[nodiscard]] bool CanPlace(const Tetromino& tetromino) const;
	[[nodiscard]] std::vector<int> GetFullRows() const;
	[[nodiscard]] const Grid& GetGrid() const;
};