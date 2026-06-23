#include "Console.h";
#include "GameOverScene.h"

void InitGameOver(GameState& state)
{
	system("cls");
	SetConsoleWindowSize(60, 30);
}

void UpdateGameOver(GameState& state)
{
	if (GetKeyDown(VK_UP))
	{
		state.curOverMenu = GameOverMenu::NEW;
	}
	if (GetKeyDown(VK_DOWN))
	{
		state.curOverMenu = GameOverMenu::TITLE;
	}
	if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
	{
		switch (state.curOverMenu)
		{
		case GameOverMenu::NEW:
			NewGame(state);
			break;
		case GameOverMenu::TITLE:
			GoToTitle(state);
			break;
		}
	}
}

void RenderGameOver(GameState& state)
{
	COORD res = GetConsoleResolution();
	int x = res.X / 2 - 4;
	int y = res.Y / 3 * 2;
	GotoXY(x - 2, y - 2);
	cout << "걸린 시간: " << state.totalTime << "초";
	const string label[] = { "새 게임", "타이틀" };
	for (int i = 0; i < 2; ++i)
	{
		GotoXY(x - 2, y + i);
		cout << (i == (int)state.curOverMenu ? "> " : "  ") << label[i];
	}
}

void NewGame(GameState& state)
{
	state.isInit = false;
	state.isFailed = false;
	state.mapLoaded = false;
	state.totalTime = 0;
	state.curScene = Scene::SELECT;
}

void GoToTitle(GameState& state)
{
	state.isInit = false;
	state.isFailed = false;
	state.mapLoaded = false;
	state.totalTime = 0;
	state.curScene = Scene::TITLE;
}
