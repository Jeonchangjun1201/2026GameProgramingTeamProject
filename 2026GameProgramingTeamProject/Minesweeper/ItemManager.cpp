#include "ItemManager.h"
#include "../Console.h"

ItemType RollRandomItem()
{
	static const ItemType pool[] = {
		ItemType::MagnifyingGlass, ItemType::Armor, ItemType::Brush,
		ItemType::Compass, ItemType::SafeScanner, ItemType::DefuseKit, ItemType::Hammer
	};
	return pool[rand() % 7];
}

void GrantItem(GameState& state, ItemType type, bool bonus)
{
	if (type == ItemType::None)
		return;
	if (!state.inventory.AddItem(type, 1))
		return;

	state.notifyItem = type;
	state.notifyBonus = bonus;
	state.itemNotifyUntil = state.curTime + ITEM_NOTIFY_MS;
	ShakeConsoleWindow(bonus ? 6 : 3, bonus ? 180 : 120, 20);
	PlaySound(L"SystemAsterisk", nullptr, SND_ASYNC | SND_ALIAS);
}

void SetupRunInventory(GameState& state)
{
	state.inventory.CopyFrom(state.savedInventory);
	state.runStartInventory.CopyFrom(state.inventory);
}

void CommitRunInventory(GameState& state)
{
	state.savedInventory.CopyFrom(state.inventory);
}

void AbandonRunInventory(GameState& state)
{
	state.inventory.CopyFrom(state.runStartInventory);
	state.brushEquipped = false;
	state.armorCount = 0;
	state.magnifierUntil = 0;
	state.compassUntil = 0;
	state.itemNotifyUntil = 0;
}

void CollectRevealedTreasures(GameState& state)
{
	for (int y = 0; y < state.board.GetHeight(); ++y)
	{
		for (int x = 0; x < state.board.GetWidth(); ++x)
		{
			Cell& cell = state.board.GetCellMut(x, y);
			if (cell.isTreasure && cell.isRevealed && !cell.treasureCollected)
			{
				cell.treasureCollected = true;
				GrantItem(state, RollRandomItem(), false);
			}
		}
	}
}

void CheckRevealBonus(GameState& state)
{
	if (state.board.GetLastRevealCount() >= FLOOD_BONUS_TILE_COUNT)
		GrantItem(state, RollRandomItem(), true);
}

bool HandleMineExplosion(GameState& state, int x, int y)
{
	if (state.armorCount > 0)
	{
		--state.armorCount;
		state.board.RecoverFromExplosion(x, y);
		ShakeConsoleWindow(8, 200, 20);
		PlaySound(L"SystemHand", nullptr, SND_ASYNC | SND_ALIAS);
		return false;
	}
	ShakeConsoleWindow(15, 400, 15);
	PlaySound(L"SystemHand", nullptr, SND_ASYNC | SND_ALIAS);
	return true;
}

static void ActivateCompass(GameState& state, int cx, int cy)
{
	static const int dirs[8][2] = {
		{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}
	};
	int bestIdx = 0;
	int bestCount = 9999;
	for (int i = 0; i < 8; ++i)
	{
		int count = state.board.CountMinesInDirection(cx, cy, dirs[i][0], dirs[i][1], 8);
		if (count < bestCount)
		{
			bestCount = count;
			bestIdx = i;
		}
	}
	state.compassDirX = dirs[bestIdx][0];
	state.compassDirY = dirs[bestIdx][1];
	state.compassUntil = state.curTime + COMPASS_DURATION_MS;
}

static bool RevealSafeWithScanner(GameState& state, int cx, int cy)
{
	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			int x = cx + dx;
			int y = cy + dy;
			if (!state.board.InBounds(x, y))
				continue;
			const Cell& cell = state.board.GetCell(x, y);
			if (!cell.isRevealed && !cell.isMine && !cell.isFlagged)
			{
				state.board.ResetRevealCount();
				state.board.RevealCell(x, y, false);
				CollectRevealedTreasures(state);
				CheckRevealBonus(state);
				return true;
			}
		}
	}
	return false;
}

static bool RevealAreaWithHammer(GameState& state, int cx, int cy)
{
	bool hitMine = false;
	int mineX = cx;
	int mineY = cy;

	state.board.ResetRevealCount();
	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			int x = cx + dx;
			int y = cy + dy;
			if (!state.board.InBounds(x, y))
				continue;
			const Cell& cell = state.board.GetCell(x, y);
			if (cell.isRevealed)
				continue;
			if (cell.isMine)
			{
				hitMine = true;
				mineX = x;
				mineY = y;
				continue;
			}
			state.board.RevealCell(x, y, false);
		}
	}

	CollectRevealedTreasures(state);
	CheckRevealBonus(state);

	if (hitMine)
	{
		state.board.RevealCell(mineX, mineY, false);
		if (HandleMineExplosion(state, mineX, mineY))
		{
			AbandonRunInventory(state);
			state.curScene = Scene::GAMEOVER;
			state.needsFullRedraw = true;
			return true;
		}
		return false;
	}
	return false;
}

static bool TryWin(GameState& state)
{
	if (!state.board.IsWin())
		return false;
	CommitRunInventory(state);
	state.curScene = Scene::WIN;
	state.needsFullRedraw = true;
	PlaySound(L"SystemAsterisk", nullptr, SND_ASYNC | SND_ALIAS);
	return true;
}

bool UseItem(GameState& state, ItemType type)
{
	if (type == ItemType::None || state.inventory.GetCount(type) <= 0)
		return false;

	int ax = state.actionCell.x;
	int ay = state.actionCell.y;
	if (!state.board.InBounds(ax, ay))
	{
		ax = state.cursor.x;
		ay = state.cursor.y;
	}

	switch (type)
	{
	case ItemType::MagnifyingGlass:
		state.magnifierUntil = state.curTime + 5000;
		state.inventory.RemoveItem(type, 1);
		break;
	case ItemType::Armor:
		if (state.armorCount >= 1)
			return false;
		state.armorCount = 1;
		state.inventory.RemoveItem(type, 1);
		break;
	case ItemType::Brush:
		if (state.brushEquipped)
		{
			state.brushEquipped = false;
			return true;
		}
		if (state.inventory.GetCount(ItemType::Brush) <= 0)
			return false;
		state.brushEquipped = true;
		return true;
	case ItemType::Compass:
		if (!state.board.InBounds(ax, ay))
			return false;
		ActivateCompass(state, ax, ay);
		state.inventory.RemoveItem(type, 1);
		break;
	case ItemType::SafeScanner:
		if (!state.board.InBounds(ax, ay))
			return false;
		if (!RevealSafeWithScanner(state, ax, ay))
			return false;
		state.inventory.RemoveItem(type, 1);
		if (TryWin(state))
			return true;
		break;
	case ItemType::DefuseKit:
		if (!state.board.InBounds(ax, ay))
			return false;
		if (!state.board.RemoveMine(ax, ay))
			return false;
		state.inventory.RemoveItem(type, 1);
		break;
	case ItemType::Hammer:
		if (!state.board.InBounds(ax, ay))
			return false;
		if (RevealAreaWithHammer(state, ax, ay))
			return true;
		state.inventory.RemoveItem(type, 1);
		if (TryWin(state))
			return true;
		break;
	default:
		return false;
	}

	PlaySound(L"SystemDefault", nullptr, SND_ASYNC | SND_ALIAS);
	if (TryWin(state))
		return true;
	return true;
}
