#include "Game.h"
#include "Console.h"
#include "GameState.h"
#include "InGame.h"

void InitGame(GameState& state)
{
	srand((unsigned int)time(nullptr));
	SetConsoleWindowSize(160, 45);
	SetConsoleWindowStyle(true);
	SetConsoleMouseInputDisabled();
	state = GameState();
}

void UpdateGame(GameState& state)
{
	bool sceneChanged = state.curScene != state.prevScene;
	state.prevScene = state.curScene;

	switch (state.curScene)
	{
	case Scene::TITLE:
		state.curScene = Scene::INGAME;
		break;
	case Scene::SELECT:
		break;
	case Scene::INGAME:
		if (sceneChanged)
			InitInGame(state);
		UpdateInGame(state);
		break;
	case Scene::GAMEOVER:
		break;
	}
}

void RenderGame(GameState& state)
{
	if (state.prevScene != state.curScene)
	{
		system("cls");
		SetConsoleMouseInputDisabled();
	}
	GotoXY(0, 0);
	switch (state.curScene)
	{
	case Scene::TITLE:
		break;
	case Scene::SELECT:
		break;
	case Scene::INGAME:
		RenderInGame(state);
		break;
	case Scene::GAMEOVER:
		break;
	}
}
