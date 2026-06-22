#pragma once
#include "../Enums.h"
#include <vector>

struct Cell
{
	bool isMine = false;
	bool isRevealed = false;
	bool isFlagged = false;
	bool isTreasure = false;
	bool isTreasureZone = false;
	bool treasureCollected = false;
	int neighborMines = 0;
};

enum class RevealResult
{
	None,
	Revealed,
	Exploded,
	TreasureFound,
	BrushFlagged
};

class Board
{
public:
	void Reset(int width, int height, int mineCount);
	void Clear();

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetMineCount() const { return mineCount; }
	int GetRemainingMines() const;
	bool IsGenerated() const { return generated; }
	bool IsGameOver() const { return gameOver; }
	bool IsWin() const;

	const Cell& GetCell(int x, int y) const;
	Cell& GetCellMut(int x, int y);

	bool InBounds(int x, int y) const;

	RevealResult RevealCell(int x, int y, bool brushMode = false);
	bool ToggleFlag(int x, int y);
	bool RemoveMine(int x, int y);
	void RecoverFromExplosion(int x, int y);

	int CountMinesInRow(int row) const;
	int CountMinesInCol(int col) const;
	int CountMinesInDirection(int x, int y, int dx, int dy, int range) const;

	bool GenerateBoard(int safeX, int safeY, int zoneCount, int treasuresPerZone);
	int GetLastRevealCount() const { return lastRevealCount; }
	void ResetRevealCount() { lastRevealCount = 0; }

private:
	static bool IsSafeZone(int x, int y, int safeX, int safeY);
	void RecalculateNeighborCounts();
	void FloodReveal(int x, int y);
	void PlaceTreasureZones(int zoneCount, int treasuresPerZone, int safeX, int safeY);
	void ClusterMinesAroundTreasures(int safeX, int safeY);

	int width = 0;
	int height = 0;
	int mineCount = 0;
	int flaggedCount = 0;
	int lastRevealCount = 0;
	bool generated = false;
	bool gameOver = false;
	std::vector<Cell> cells;
};
