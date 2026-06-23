#include "Game.h"
#include "Console.h"
#include "GameState.h"
#include "InGame.h"
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameOverScene.h"
#include "SoundManager.h"
void InitGame(GameState& state)
{
	srand((unsigned int)time(nullptr));
	SetConsoleWindowSize(160, 45);
	SetConsoleWindowStyle(true);
	SetConsoleMouseInputDisabled();
	state = GameState();
	SOUND->Init();
	SOUND->Load("mineExplosion", "mineExplosion.wav");
	SOUND->Load("tile", "tile.wav");
	SOUND->Load("flag", "flag.wav");
}

void UpdateGame(GameState& state)
{
	bool sceneChanged = state.curScene != state.prevScene;
	state.prevScene = state.curScene;

	switch (state.curScene)
	{
	case Scene::TITLE:
		if (sceneChanged)
			InitTitle(state);
		UpdateTitle(state);
		break;
	case Scene::SELECT:
		if (sceneChanged)
			InitSelect(state);
		UpdateSelect(state);
		break;
	case Scene::INGAME:
		if (sceneChanged)
			InitInGame(state);
		UpdateInGame(state);
		break;
	case Scene::GAMEOVER:
		if (sceneChanged)
			InitGameOver(state);
		UpdateGameOver(state);
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
		RenderTitle(state);
		break;
	case Scene::SELECT:
		RenderSelect(state);
		break;
	case Scene::INGAME:
		RenderInGame(state);
		break;
	case Scene::GAMEOVER:
		RenderGameOver(state);
		break;
	}
}
