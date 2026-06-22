#include "Board.h"
#include <cstdlib>
#include <queue>
#include <set>
#include <utility>
#include <vector>

void Board::Reset(int w, int h, int mines)
{
	width = w;
	height = h;
	mineCount = mines;
	flaggedCount = 0;
	generated = false;
	gameOver = false;
	cells.assign(static_cast<size_t>(width * height), Cell{});
}

void Board::Clear()
{
	width = 0;
	height = 0;
	mineCount = 0;
	flaggedCount = 0;
	generated = false;
	gameOver = false;
	cells.clear();
}

bool Board::InBounds(int x, int y) const
{
	return x >= 0 && x < width && y >= 0 && y < height;
}

const Cell& Board::GetCell(int x, int y) const
{
	return cells[static_cast<size_t>(y * width + x)];
}

Cell& Board::GetCellMut(int x, int y)
{
	return cells[static_cast<size_t>(y * width + x)];
}

int Board::GetRemainingMines() const
{
	return (mineCount - flaggedCount) > 0 ? (mineCount - flaggedCount) : 0;
}

bool Board::IsWin() const
{
	if (!generated || gameOver)
		return false;

	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const Cell& cell = GetCell(x, y);
			if (cell.isMine)
				continue;
			if (!cell.isRevealed)
				return false;
		}
	}
	return true;
}

bool Board::IsSafeZone(int x, int y, int safeX, int safeY)
{
	return x >= safeX - 1 && x <= safeX + 1 && y >= safeY - 1 && y <= safeY + 1;
}

bool Board::GenerateBoard(int safeX, int safeY, int zoneCount, int treasuresPerZone)
{
	if (!InBounds(safeX, safeY))
		return false;

	std::vector<int> candidates;
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (IsSafeZone(x, y, safeX, safeY))
				continue;
			candidates.push_back(y * width + x);
		}
	}

	if (static_cast<int>(candidates.size()) < mineCount)
		return false;

	for (int i = static_cast<int>(candidates.size()) - 1; i > 0; --i)
	{
		int j = rand() % (i + 1);
		std::swap(candidates[i], candidates[j]);
	}

	for (int i = 0; i < mineCount; ++i)
		cells[static_cast<size_t>(candidates[static_cast<size_t>(i)])].isMine = true;

	PlaceTreasureZones(zoneCount, treasuresPerZone, safeX, safeY);
	ClusterMinesAroundTreasures(safeX, safeY);
	RecalculateNeighborCounts();
	lastRevealCount = 0;
	generated = true;
	return true;
}

void Board::PlaceTreasureZones(int zoneCount, int treasuresPerZone, int safeX, int safeY)
{
	const int radius = TREASURE_ZONE_RADIUS;

	for (int t = 0; t < zoneCount; ++t)
	{
		for (int attempt = 0; attempt < 200; ++attempt)
		{
			int cx = rand() % width;
			int cy = rand() % height;

			std::vector<std::pair<int, int>> zoneCells;
			bool invalid = false;
			for (int dy = -radius; dy <= radius; ++dy)
			{
				for (int dx = -radius; dx <= radius; ++dx)
				{
					int nx = cx + dx;
					int ny = cy + dy;
					if (!InBounds(nx, ny))
					{
						invalid = true;
						break;
					}
					if (IsSafeZone(nx, ny, safeX, safeY))
					{
						invalid = true;
						break;
					}
					zoneCells.push_back(std::make_pair(nx, ny));
				}
				if (invalid)
					break;
			}
			if (invalid || zoneCells.empty())
				continue;

			for (size_t i = 0; i < zoneCells.size(); ++i)
				GetCellMut(zoneCells[i].first, zoneCells[i].second).isTreasureZone = true;

			std::vector<std::pair<int, int>> treasureCandidates;
			for (size_t i = 0; i < zoneCells.size(); ++i)
			{
				Cell& c = GetCellMut(zoneCells[i].first, zoneCells[i].second);
				if (!c.isMine && !c.isTreasure)
					treasureCandidates.push_back(zoneCells[i]);
			}
			if (treasureCandidates.empty())
				continue;

			int placeCount = treasuresPerZone;
			if (placeCount > static_cast<int>(treasureCandidates.size()))
				placeCount = static_cast<int>(treasureCandidates.size());

			for (int p = 0; p < placeCount; ++p)
			{
				int pick = rand() % static_cast<int>(treasureCandidates.size());
				std::pair<int, int> pos = treasureCandidates[static_cast<size_t>(pick)];
				GetCellMut(pos.first, pos.second).isTreasure = true;
				treasureCandidates.erase(treasureCandidates.begin() + pick);
			}
			break;
		}
	}
}

void Board::ClusterMinesAroundTreasures(int safeX, int safeY)
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			const Cell& source = GetCell(x, y);
			if (!source.isTreasure)
				continue;

			for (int dy = -2; dy <= 2; ++dy)
			{
				for (int dx = -2; dx <= 2; ++dx)
				{
					if (dx == 0 && dy == 0)
						continue;

					int nx = x + dx;
					int ny = y + dy;
					if (!InBounds(nx, ny))
						continue;

					Cell& target = GetCellMut(nx, ny);
					if (target.isMine || target.isTreasure)
						continue;
					if (IsSafeZone(nx, ny, safeX, safeY))
						continue;

					if (rand() % 100 < TREASURE_ZONE_CLUSTER_CHANCE)
					{
						for (int sy = 0; sy < height; ++sy)
						{
							for (int sx = 0; sx < width; ++sx)
							{
								Cell& mineCell = GetCellMut(sx, sy);
								if (!mineCell.isMine)
									continue;
								if (IsSafeZone(sx, sy, safeX, safeY))
									continue;

								int distToTreasure = abs(sx - x) + abs(sy - y);
								int distCandidate = abs(nx - sx) + abs(ny - sy);
								if (distToTreasure > distCandidate + 1)
								{
									mineCell.isMine = false;
									target.isMine = true;
									goto nextCluster;
								}
							}
						}
					}
				nextCluster:;
				}
			}
		}
	}
}

void Board::RecalculateNeighborCounts()
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			Cell& cell = GetCellMut(x, y);
			if (cell.isMine)
			{
				cell.neighborMines = -1;
				continue;
			}

			int count = 0;
			for (int dy = -1; dy <= 1; ++dy)
			{
				for (int dx = -1; dx <= 1; ++dx)
				{
					if (dx == 0 && dy == 0)
						continue;
					int nx = x + dx;
					int ny = y + dy;
					if (InBounds(nx, ny) && GetCell(nx, ny).isMine)
						++count;
				}
			}
			cell.neighborMines = count;
		}
	}
}

RevealResult Board::RevealCell(int x, int y, bool brushMode)
{
	if (!generated || gameOver || !InBounds(x, y))
		return RevealResult::None;

	Cell& cell = GetCellMut(x, y);
	if (cell.isRevealed || cell.isFlagged)
		return RevealResult::None;

	if (cell.isMine)
	{
		if (brushMode)
		{
			cell.isFlagged = true;
			++flaggedCount;
			return RevealResult::BrushFlagged;
		}
		cell.isRevealed = true;
		gameOver = true;
		return RevealResult::Exploded;
	}

	cell.isRevealed = true;
	++lastRevealCount;
	if (cell.isTreasure && !cell.treasureCollected)
	{
		FloodReveal(x, y);
		return RevealResult::TreasureFound;
	}
	if (cell.neighborMines == 0)
		FloodReveal(x, y);
	return RevealResult::Revealed;
}

void Board::FloodReveal(int x, int y)
{
	std::queue<std::pair<int, int>> q;
	q.push(std::make_pair(x, y));
	std::set<std::pair<int, int>> visited;
	visited.insert(std::make_pair(x, y));

	while (!q.empty())
	{
		int cx = q.front().first;
		int cy = q.front().second;
		q.pop();

		for (int dy = -1; dy <= 1; ++dy)
		{
			for (int dx = -1; dx <= 1; ++dx)
			{
				if (dx == 0 && dy == 0)
					continue;
				int nx = cx + dx;
				int ny = cy + dy;
				if (!InBounds(nx, ny))
					continue;

				Cell& next = GetCellMut(nx, ny);
				if (next.isRevealed || next.isFlagged || next.isMine)
					continue;

				next.isRevealed = true;
				++lastRevealCount;
				if (next.neighborMines == 0)
				{
					std::pair<int, int> key = std::make_pair(nx, ny);
					if (visited.insert(key).second)
						q.push(key);
				}
			}
		}
	}
}

bool Board::ToggleFlag(int x, int y)
{
	if (!generated || gameOver || !InBounds(x, y))
		return false;

	Cell& cell = GetCellMut(x, y);
	if (cell.isRevealed)
		return false;

	if (cell.isFlagged)
	{
		cell.isFlagged = false;
		--flaggedCount;
	}
	else
	{
		cell.isFlagged = true;
		++flaggedCount;
	}
	return true;
}

void Board::RecoverFromExplosion(int x, int y)
{
	gameOver = false;
	if (InBounds(x, y))
		GetCellMut(x, y).isRevealed = false;
}

bool Board::RemoveMine(int x, int y)
{
	if (!generated || gameOver || !InBounds(x, y))
		return false;

	Cell& cell = GetCellMut(x, y);
	if (!cell.isMine || !cell.isFlagged)
		return false;

	cell.isMine = false;
	cell.isFlagged = false;
	cell.isRevealed = true;
	--flaggedCount;
	--mineCount;
	RecalculateNeighborCounts();
	return true;
}

int Board::CountMinesInRow(int row) const
{
	if (row < 0 || row >= height)
		return 0;
	int count = 0;
	for (int x = 0; x < width; ++x)
		if (GetCell(x, row).isMine)
			++count;
	return count;
}

int Board::CountMinesInCol(int col) const
{
	if (col < 0 || col >= width)
		return 0;
	int count = 0;
	for (int y = 0; y < height; ++y)
		if (GetCell(col, y).isMine)
			++count;
	return count;
}

int Board::CountMinesInDirection(int x, int y, int dx, int dy, int range) const
{
	int count = 0;
	int cx = x + dx;
	int cy = y + dy;
	for (int i = 0; i < range; ++i)
	{
		if (!InBounds(cx, cy))
			break;
		if (GetCell(cx, cy).isMine)
			++count;
		cx += dx;
		cy += dy;
	}
	return count;
}
