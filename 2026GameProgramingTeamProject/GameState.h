#pragma once
#include <vector>
#include "Enums.h"
#include <Windows.h>

constexpr int EASY_MAP_HEIGHT = 9;
constexpr int EASY_MAP_WIDTH = 9;
constexpr int NORMAL_MAP_HEIGHT = 16;
constexpr int NORMAL_MAP_WIDTH = 16;
constexpr int HARD_MAP_HEIGHT = 16;
constexpr int HARD_MAP_WIDTH = 30;
constexpr int MAX_MAP_HEIGHT = 30;
constexpr int MAX_MAP_WIDTH = 30;
constexpr int DX[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
constexpr int DY[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };


struct Mine
{
	POINT pos = { 0, 0 };
	bool active = false;
};

struct GameState
{
public:
	Scene prevScene = Scene::NONE;
	Scene curScene = Scene::TITLE;
	Menu curMenu = Menu::START;
	Difficulty curDiff = Difficulty::NORMAL;
	bool isRunning = true;
	bool isInit = false;
	POINT startPos = {};
	Block map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {};
	Block realMap[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {};
	ULONGLONG curTime;
	int maxMineCount = 0;
	int mapH = 0;
	int mapW = 0;
	std::vector<Mine> vecMine;
};
