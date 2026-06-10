#pragma once
#include <vector>
#include <string>
#include "Enums.h"

constexpr float NORMAL_MIN_VALUE = 0.5f;
constexpr float NORMAL_MAX_VALUE = 2;
constexpr float NEWS_MIN_VALUE = 10;
constexpr float NEWS_MAX_VALUE = 20;

struct GameState
{
	Scene prevScene = Scene::NONE;
	Scene curScene = Scene::TITLE;
	Menu curMenu = Menu::START;
	Difficulty curDiff = Difficulty::EASY;
	bool isRunning = true;
};
