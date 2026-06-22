#pragma once
#include "Enums.h"
#include "Minesweeper/Board.h"
#include "Minesweeper/ItemInventory.h"
#include <Windows.h>

struct GameState
{
	Scene curScene = Scene::TITLE;
	Difficulty curDiff = Difficulty::EASY;
	DifficultyConfig config{};
	bool isRunning = true;

	Board board;
	ItemInventory inventory;
	ItemInventory savedInventory;
	ItemInventory runStartInventory;

	POINT cursor{};
	POINT actionCell{};
	bool firstClick = true;
	bool gameStarted = false;
	bool brushEquipped = false;
	int armorCount = 0;

	ULONGLONG magnifierUntil = 0;
	int compassDirX = 0;
	int compassDirY = 0;
	ULONGLONG compassUntil = 0;

	ItemType notifyItem = ItemType::None;
	bool notifyBonus = false;
	ULONGLONG itemNotifyUntil = 0;

	ULONGLONG curTime = 0;
	bool needsFullRedraw = true;
};
