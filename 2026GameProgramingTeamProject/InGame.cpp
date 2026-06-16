#include "InGame.h"
#include <fstream>
#include "Console.h"
#include <algorithm>

void LoadMap(GameState& state)
{
	std::ifstream mapFile;
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
		break;
	}
	}
}

void DrawMap(const GameState& state)
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

void DrawTile(const GameState& state, int x, int y)
{
	switch (state.map[y][x])
	{
	case Block::TILE:
		SetColor(Color::GRAY);
		cout << "#";
		break;
	case Block::EMPTY:
		SetColor();
		cout << " ";
		break;
	case Block::MINE:
		SetColor(Color::RED);
		{
			cout << "¡Ø";
		}
		break;
	}
}

void InitInGame(GameState& state)
{
	LoadMap(state);
}

void UpdateInGame(GameState& state)
{

}

void RenderInGame(GameState& state)
{
	DrawMap(state);
}
