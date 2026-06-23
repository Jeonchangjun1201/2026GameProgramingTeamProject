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
constexpr int EXTREME_MAP_HEIGHT = 30;
constexpr int EXTREME_MAP_WIDTH = 30;
constexpr int MAX_MAP_HEIGHT = 30;
constexpr int MAX_MAP_WIDTH = 30;
constexpr int DX[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
constexpr int DY[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
constexpr int TIMER_MS = 1000;

struct GameState
{
public:
	Scene prevScene = Scene::NONE;
	Scene curScene = Scene::TITLE;
	Menu curMenu = Menu::START;
	GameOverMenu curOverMenu = GameOverMenu::NEW;
	Difficulty curDiff = Difficulty::NORMAL;
	bool isRunning = true;
	bool isInit = false;
	bool isFailed = false;
	bool isProtection = false;
	bool isBrush = false;
	bool isMagnify = false;
	bool mapLoaded = false;
	POINT startPos = {};
	Block map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {};
	Block realMap[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = {};
	ULONGLONG lastTime = 0;
	ULONGLONG nextCountTime = 0;
	ULONGLONG totalTime = 0;
	bool timeStopped = true;
	int maxMineCount = 0;
	int maxFlagCount = 0;
	int canPlaceFlagCount = 0;
	int mapH = 0;
	int mapW = 0;
	POINT magItemPos = {};
	POINT protecItemPos = {};
	POINT brushItemPos = {};
	bool hasProtection = false;
	bool hasBrush = false;
	bool hasMagnify = false;
	bool protectionRevealed = false;
	bool brushRevealed = false;
	bool magnifyRevealed = false;
	bool magnifyUsed = false;
	POINT magnifyResult = {};
	POINT magnifyRange = {};
};
