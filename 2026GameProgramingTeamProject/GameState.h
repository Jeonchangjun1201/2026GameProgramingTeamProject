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


struct Mine
{
	COORD pos = { 0, 0 };
	bool active = false;
};

struct GameState
{
public:
	Scene prevScene = Scene::NONE;
	Scene curScene = Scene::TITLE;
	Menu curMenu = Menu::START;
	Difficulty curDiff = Difficulty::HARD;
	bool isRunning = true;
	Block map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {};
	ULONGLONG curTime;
	int mapH = 0;
	int mapW = 0;
	std::vector<Mine> vecMine;
};
