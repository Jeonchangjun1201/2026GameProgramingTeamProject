#include "InGame.h"
#include <fstream>
#include "Console.h"
#include <algorithm>

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
	}
	for (int y = 0; y < state.mapH; ++y)
	{
		for (int x = 0; x < state.mapW; ++x)
		{
			state.realMap[y][x] = Block::EMPTY;
		}
	}
}

void DrawMap(GameState& state)
{
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
	POINT tilePos;
	while (placedCnt <= state.maxMineCount)
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

void RevealTile(GameState& state, int x, int y)
{
	if (state.map[y][x] == Block::FLAG) return;
	state.map[y][x] = state.realMap[y][x];
	if (state.realMap[y][x] == Block::MINE)
	{
		ShakeConsoleWindow(8, 500, 25);
		for (int ry = 0; ry < state.mapH; ry++)
		{
			for (int rx = 0; rx < state.mapW; rx++)
			{
				if(state.realMap[ry][rx] == Block::MINE)
					state.map[ry][rx] = state.realMap[ry][rx];
			}
		}
	}
}

bool IsInRange(GameState& state, int x, int y)
{
	return x >= 0 && x < state.mapW && y >= 0 && y < state.mapH ? true : false;
}

void InitInGame(GameState& state)
{
	LoadMap(state);
}

void UpdateInGame(GameState& state)
{
	UpdateInput();
	if (GetKeyDown(VK_LBUTTON) && !state.isInit)
	{
		state.startPos = GetAndAdjustPosition();
		PlaceMine(state);
		state.isInit = true;
	}
	if (GetKeyDown(VK_LBUTTON))
	{
		POINT temp = GetAndAdjustPosition();
		RevealTile(state, temp.x, temp.y);
	}
	if (GetKeyDown(VK_RBUTTON) || GetKeyDown('F'))
	{
		POINT temp = GetAndAdjustPosition();
		if (state.map[temp.y][temp.x] == Block::TILE)
			state.map[temp.y][temp.x] = Block::FLAG;
		else if (state.map[temp.y][temp.x] == Block::FLAG)
			state.map[temp.y][temp.x] = Block::TILE;
	}
}

void RenderInGame(GameState& state)
{
	DrawMap(state);
}
