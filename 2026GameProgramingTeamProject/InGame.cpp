#include "InGame.h"
#include <fstream>
#include "Console.h"
#include <algorithm>
#include "SoundManager.h"

namespace
{
	bool IsItemNotifyActive(ULONGLONG endTime)
	{
		return GetTickCount64() < endTime;
	}

	bool IsItemBlinkOn()
	{
		return (GetTickCount64() / ITEM_BLINK_INTERVAL_MS) % 2 == 0;
	}

	void StartItemNotify(ULONGLONG& notifyEndTime)
	{
		notifyEndTime = GetTickCount64() + ITEM_NOTIFY_DURATION_MS;
	}

	bool TryDrawItemSparkle(int x, int y, const POINT& itemPos, ULONGLONG notifyEndTime)
	{
		if (x != itemPos.x || y != itemPos.y)
			return false;
		if (!IsItemNotifyActive(notifyEndTime))
			return false;

		if (IsItemBlinkOn())
		{
			SetColor(Color::LIGHT_YELLOW);
			cout << "★";
		}
		else
		{
			SetColor(Color::LIGHT_VIOLET);
			cout << "◆";
		}
		SetColor();
		return true;
	}

	void DrawItemLine(int uiX, int uiY, ULONGLONG notifyEndTime, bool hasItem, bool isUsing, const string& ownedText, const string& usingText, const string& emptyText)
	{
		GotoXY(uiX, uiY);
		if (IsItemNotifyActive(notifyEndTime) && IsItemBlinkOn())
			SetColor(Color::LIGHT_YELLOW);
		else
			SetColor();

		if (hasItem)
			cout << ownedText;
		else if (isUsing)
			cout << usingText;
		else
			cout << emptyText;

		SetColor();
	}
}

void LoadMap(GameState& state)
{
	switch (state.curDiff)
	{
	case Difficulty::EASY:
	{
		std::ifstream mapFile("TextFiles\\EasyMap.txt");
		if (!mapFile.is_open())
			return;
		state.mapW = EASY_MAP_WIDTH;
		state.mapH = EASY_MAP_HEIGHT;
		for (int y = 0; y < state.mapH; ++y)
		{
			string line;
			mapFile >> line;
			for (int x = 0; x < state.mapW; ++x)
			{
				int data = line[x] - '0';
				state.map[y][x] = (Block)data;
			}
		}
		state.maxMineCount = 10;
		break;
	}
	case Difficulty::NORMAL:
	{
		std::ifstream mapFile("TextFiles\\NormalMap.txt");
		if (!mapFile.is_open())
			return;
		state.mapW = NORMAL_MAP_WIDTH;
		state.mapH = NORMAL_MAP_HEIGHT;
		for (int y = 0; y < state.mapH; ++y)
		{
			string line;
			mapFile >> line;
			for (int x = 0; x < state.mapW; ++x)
			{
				int data = line[x] - '0';
				state.map[y][x] = (Block)data;
			}
		}
		state.maxMineCount = 40;
		break;
	}
	case Difficulty::HARD:
	{
		std::ifstream mapFile("TextFiles\\HardMap.txt");
		if (!mapFile.is_open())
			return;
		state.mapW = HARD_MAP_WIDTH;
		state.mapH = HARD_MAP_HEIGHT;
		for (int y = 0; y < state.mapH; ++y)
		{
			string line;
			mapFile >> line;
			for (int x = 0; x < state.mapW; ++x)
			{
				int data = line[x] - '0';
				state.map[y][x] = (Block)data;
			}
		}
		state.maxMineCount = 99;
		break;
	}
	case Difficulty::EXTREME:
	{
		std::ifstream mapFile("TextFiles\\ExtremeMap.txt");
		if (!mapFile.is_open())
			return;
		state.mapW = EXTREME_MAP_WIDTH;
		state.mapH = EXTREME_MAP_HEIGHT;
		for (int y = 0; y < state.mapH; ++y)
		{
			string line;
			mapFile >> line;
			for (int x = 0; x < state.mapW; ++x)
			{
				int data = line[x] - '0';
				state.map[y][x] = (Block)data;
			}
		}
		state.maxMineCount = 199;
		break;
	}
	}
	state.maxFlagCount = state.maxMineCount;
	SetConsoleWindowSize(state.mapW * 2 + 20, state.mapH + 15);
	for (int y = 0; y < state.mapH; ++y)
	{
		for (int x = 0; x < state.mapW; ++x)
		{
			state.realMap[y][x] = Block::EMPTY;
		}
	}
	state.mapLoaded = true;
}

void DrawMap(GameState& state)
{
	GotoXY(0, 0);
	for (int y = 0; y < state.mapH; ++y)
	{
		for (int x = 0; x < state.mapW; ++x)
		{
			DrawTile(state, x, y);
		}
		cout << endl;
	}
	SetColor();
}

void DrawTile(GameState& state, int x, int y)
{
	if (TryDrawItemSparkle(x, y, state.protecItemPos, state.protectionNotifyEndTime))
		return;
	if (TryDrawItemSparkle(x, y, state.brushItemPos, state.brushNotifyEndTime))
		return;
	if (TryDrawItemSparkle(x, y, state.magItemPos, state.magnifyNotifyEndTime))
		return;

	switch (state.map[y][x])
	{
	case Block::TILE:
		SetColor(Color::GRAY);
		cout << "■";
		break;
	case Block::EMPTY:
		DrawNumber(state, x, y);
		break;
	case Block::MINE:
		SetColor(Color::RED);
		cout << "※";
		break;
	case Block::FLAG:
	{
		SetColor(Color::LIGHT_YELLOW);
		cout << "▣";
		break;
	}
	}
}

void DrawNumber(GameState& state, int x, int y)
{
	int cnt = CheckMinesInRange(state, x, y);
	int newX;
	int newY;
	switch (cnt)
	{
	case 1:
		SetColor(Color::LIGHT_BLUE);
		cout << "１";
		break;
	case 2:
		SetColor(Color::LIGHT_GREEN);
		cout << "２";
		break;
	case 3:
		SetColor(Color::LIGHT_RED);
		cout << "３";
		break;
	case 4:
		SetColor(Color::BLUE);
		cout << "４";
		break;
	case 5:
		SetColor(Color::RED);
		cout << "５";
		break;
	case 6:
		SetColor(Color::CYAN);
		cout << "６";
		break;
	case 7:
		SetColor(Color::VIOLET);
		cout << "７";
		break;
	case 8:
		SetColor(Color::GRAY);
		cout << "８";
		break;
	default:
		for (int i = 0; i < 8; i++)
		{
			newX = x + DX[i];
			newY = y + DY[i];

			if (IsInRange(state, newX, newY) == false) continue;
			RevealTile(state, newX, newY);
		}
		cout << "  ";
	}
	SetColor();
}

void PlaceMine(GameState& state)
{
	int placedCnt = 0;
	int tileX = 0;
	int tileY = 0;
	while (placedCnt < state.maxMineCount)
	{
		tileX = rand() % state.mapW;
		tileY = rand() % state.mapH;

		if (state.realMap[tileY][tileX] == Block::MINE || (tileX == state.startPos.x && tileY == state.startPos.y)) continue;
		else {
			placedCnt++;
			state.realMap[tileY][tileX] = Block::MINE;
		}
	}
}

void PlaceItems(GameState& state)
{
	bool val = false;
	int tileX = 0;
	int tileY = 0;
	int temp = 0;
	int num = 0;
	POINT tempPos = {};
	// protection 아이템
	for (int ry = 0; ry < state.mapH; ry++)
	{
		for (int rx = 0; rx < state.mapW; rx++)
		{
			if (state.realMap[ry][rx] == Block::MINE) continue;
			temp = CheckMinesInRange(state, rx, ry);
			if (temp > num)
			{
				tempPos = { rx, ry };
				num = temp;
			}
		}
	}
	state.protecItemPos = tempPos;

	// brush 아이템
	temp = 0;
	num = 0;
	for (int ry = 0; ry < state.mapH; ry++)
	{
		for (int rx = 0; rx < state.mapW; rx++)
		{
			if (state.realMap[ry][rx] == Block::MINE) continue;
			if ((rx == state.protecItemPos.x) && ry == (state.protecItemPos.y)) continue;
			temp = CheckMinesInRange(state, rx, ry);
			if (temp >= num)
			{
				tempPos = { rx, ry };
				num = temp;
			}
		}
	}

	state.brushItemPos = tempPos;

	// magnify 아이템
	temp = 0;
	num = 0;
	for (int ry = 0; ry < state.mapH; ry++)
	{
		for (int rx = 0; rx < state.mapW; rx++)
		{
			if (state.realMap[ry][rx] == Block::MINE) continue;
			if ((rx == state.protecItemPos.x) && ry == (state.protecItemPos.y)) continue;
			if ((rx == state.brushItemPos.x) && ry == (state.brushItemPos.y)) continue;
			temp = CheckMinesInRange(state, rx, ry);
			if (temp > num)
			{
				tempPos = { rx, ry };
				num = temp;
			}
		}
	}

	state.magItemPos = tempPos;
}

void RevealTile(GameState& state, int x, int y)
{
	if (!IsInRange(state, x, y)) return;
	if (state.map[y][x] == Block::FLAG) return;
	state.map[y][x] = state.realMap[y][x];
	if (state.realMap[y][x] == Block::MINE)
	{
		if (state.isBrush)
		{
			state.isBrush = false;
			state.map[y][x] = Block::FLAG;
		}
		else
			ActivatedMine(state);
	}
}

void ActivatedMine(GameState& state)
{
	ShakeConsoleWindow(8, 500, 25);
	SOUND->Play("mineExplosion");
	if (state.isProtection)
	{
		state.isProtection = false;
		return;
	}
	for (int ry = 0; ry < state.mapH; ry++)
	{
		for (int rx = 0; rx < state.mapW; rx++)
		{
			if (state.realMap[ry][rx] == Block::MINE)
				state.map[ry][rx] = state.realMap[ry][rx];
		}
	}
	GotoXY(0, 0);
	DrawMap(state);
	state.isFailed = true;
	state.timeStopped = true;
	Sleep(2000);
	state.curScene = Scene::GAMEOVER;
}

void UpdateCurrentFlags(GameState& state)
{
	int cnt = state.maxFlagCount;
	for (int ry = 0; ry < state.mapH; ry++)
	{
		for (int rx = 0; rx < state.mapW; rx++)
		{
			if (state.map[ry][rx] == Block::FLAG)
				cnt--;
		}
	}
	state.canPlaceFlagCount = cnt;
}

bool IsInRange(GameState& state, int x, int y)
{
	return x >= 0 && x < state.mapW && y >= 0 && y < state.mapH ? true : false;
}

bool IsAllTileClear(GameState& state)
{
	int cnt = 0;
	for (int ry = 0; ry < state.mapH; ry++)
	{
		for (int rx = 0; rx < state.mapW; rx++)
		{
			if ((state.map[ry][rx] == Block::TILE || state.map[ry][rx] == Block::FLAG) && state.realMap[ry][rx] != Block::MINE)
				cnt++;
		}
	}
	return cnt == 0 ? true : false;
}

void DrawUI(GameState& state)
{
	const int UI_X = 2;
	const int UI_Y = state.mapH + 2;

	GotoXY(UI_X, UI_Y - 1);
	cout << "Time: " << state.totalTime;

	GotoXY(UI_X, UI_Y);
	cout << "Flags Left: " << state.canPlaceFlagCount << "     ";

	DrawItemLine(UI_X, UI_Y + 1, state.protectionNotifyEndTime, state.hasProtection, state.isProtection,
		"[1] - 보호구(소유 중)               ",
		"[1] - 보호구(사용 중)               ",
		"[1] - 보호구(보유하지 않음)               ");
	DrawItemLine(UI_X, UI_Y + 2, state.brushNotifyEndTime, state.hasBrush, state.isBrush,
		"[2] - 브러쉬(소유 중)               ",
		"[2] - 브러쉬(사용 중)               ",
		"[2] - 브러쉬(보유하지 않음)               ");
	DrawItemLine(UI_X, UI_Y + 3, state.magnifyNotifyEndTime, state.hasMagnify, state.isMagnify,
		"[3] - 돋보기(소유 중)               ",
		"[3] - 돋보기(사용 중)               ",
		"[3] - 돋보기(보유하지 않음)               ");

	GotoXY(UI_X, UI_Y + 5);
	if (state.magnifyUsed)
	{
		cout << "지뢰: 행(" << state.magnifyRange.y + 1 << ") - " << state.magnifyResult.x << "개, 열(" << state.magnifyRange.x + 1 << ") - " << state.magnifyResult.y << "개";
		GotoXY(UI_X, UI_Y + 6);
		SetColor(Color::LIGHT_RED);
		cout << "※행은 가로, 열은 세로줄※";
		GotoXY(UI_X, UI_Y + 7);
		cout << "※아래/오른쪽일 수록 숫자가 큼※";
		GotoXY(UI_X, UI_Y + 8);
		cout << "※인덱스 숫자 아님(1부터 시작)※";
	}
	SetColor();
}

int CheckMinesInRange(GameState& state, int x, int y)
{
	int cnt = 0;
	int newX;
	int newY;
	for (int i = 0; i < 8; i++)
	{
		newX = x + DX[i];
		newY = y + DY[i];

		if (IsInRange(state, newX, newY) == false) continue;
		if (state.realMap[newY][newX] == Block::MINE)
			cnt++;
	}
	return cnt;
}

void GrantItem(GameState& state)
{
	if (!state.protectionRevealed && state.map[state.protecItemPos.y][state.protecItemPos.x] == Block::EMPTY)
	{
		state.hasProtection = true;
		state.protectionRevealed = true;
		StartItemNotify(state.protectionNotifyEndTime);
		SOUND->Play("itemGet");
	}
	if (!state.brushRevealed && state.map[state.brushItemPos.y][state.brushItemPos.x] == Block::EMPTY)
	{
		state.hasBrush = true;
		state.brushRevealed = true;
		StartItemNotify(state.brushNotifyEndTime);
		SOUND->Play("itemGet");
	}
	if (!state.magnifyRevealed && state.map[state.magItemPos.y][state.magItemPos.x] == Block::EMPTY)
	{
		state.hasMagnify = true;
		state.magnifyRevealed = true;
		StartItemNotify(state.magnifyNotifyEndTime);
		SOUND->Play("itemGet");
	}
}

void UseMagnify(GameState& state, int x, int y)
{
	int yCnt = 0;
	int xCnt = 0;
	for (int rx = 0; rx < state.mapW; rx++)
	{
		if (state.realMap[y][rx] == Block::MINE)
			xCnt++;
	}
	for (int ry = 0; ry < state.mapH; ry++)
	{
		if (state.realMap[ry][x] == Block::MINE)
			yCnt++;
	}

	state.magnifyResult = { xCnt, yCnt };
	state.magnifyRange = { x, y };
	state.magnifyUsed = true;
}

void InitInGame(GameState& state)
{
	state.protectionNotifyEndTime = 0;
	state.brushNotifyEndTime = 0;
	state.magnifyNotifyEndTime = 0;
	LoadMap(state);
}

void UpdateInGame(GameState& state)
{
	UpdateCurrentFlags(state);
	GrantItem(state);
	if (GetKeyDown(VK_LBUTTON) && !state.isInit)
	{
		state.startPos = GetAndAdjustPosition();
		if (!IsInRange(state, state.startPos.x, state.startPos.y)) return;
		PlaceMine(state);
		PlaceItems(state);
		state.isInit = true;
		state.timeStopped = false;
		state.lastTime = GetTickCount64();
		state.nextCountTime = state.lastTime + TIMER_MS;
	}
	if (GetKeyDown(VK_LBUTTON))
	{
		POINT temp = GetAndAdjustPosition();
		if (!IsInRange(state, temp.x, temp.y)) return;
		if (state.isMagnify)
		{
			state.isMagnify = false;
			UseMagnify(state, temp.x, temp.y);
			return;
		}
		SOUND->Play("tile");
		RevealTile(state, temp.x, temp.y);
	}
	if (GetKeyDown(VK_RBUTTON) || GetKeyDown('F'))
	{
		if (state.canPlaceFlagCount == 0) return;
		POINT temp = GetAndAdjustPosition();
		if (!IsInRange(state, temp.x, temp.y)) return;
		if (state.map[temp.y][temp.x] == Block::TILE)
		{
			state.map[temp.y][temp.x] = Block::FLAG;
			SOUND->Play("flag");
		}
		else if (state.map[temp.y][temp.x] == Block::FLAG)
			state.map[temp.y][temp.x] = Block::TILE;
	}
	if (GetKeyDown('1') && state.hasProtection)
	{
		state.isProtection = true;
		state.hasProtection = false;
	}
	if (GetKeyDown('2') && state.hasBrush)
	{
		state.isBrush = true;
		state.hasBrush = false;
	}
	if (GetKeyDown('3') && state.hasMagnify)
	{
		state.isMagnify = true;
		state.hasMagnify = false;
	}
	if (IsAllTileClear(state))
	{
		state.timeStopped = true;
		Sleep(1000);
		state.curScene = Scene::GAMEOVER;
	}
	if (state.lastTime >= state.nextCountTime && state.isInit)
	{
		state.nextCountTime = state.lastTime + TIMER_MS;
		state.totalTime++;
	}
}

void RenderInGame(GameState& state)
{
	if (!state.mapLoaded) return;
	DrawMap(state);
	DrawUI(state);
}
