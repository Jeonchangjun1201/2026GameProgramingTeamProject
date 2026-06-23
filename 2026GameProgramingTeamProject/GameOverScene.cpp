#include "Console.h";
#include "GameOverScene.h"
#include "SoundManager.h"

void InitGameOver(GameState& state)
{
	system("cls");
	SetConsoleWindowSize(60, 30);
	if (state.isFailed)
		SOUND->Play("gameOver");
	else
		SOUND->Play("gameClear");
}

void UpdateGameOver(GameState& state)
{
	if (GetKeyDown(VK_UP))
	{
		SOUND->Play("move");
		state.curOverMenu = GameOverMenu::NEW;
	}
	if (GetKeyDown(VK_DOWN))
	{
		SOUND->Play("move");
		state.curOverMenu = GameOverMenu::TITLE;
	}
	if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
	{
		SOUND->Play("select");
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
	GotoXY(x - 4, y - 2);
	cout << "걸린 시간: " << state.totalTime << "초";
	const string label[] = { "새 게임", "타이틀" };
	for (int i = 0; i < 2; ++i)
	{
		GotoXY(x - 2, y + i);
		cout << (i == (int)state.curOverMenu ? "> " : "  ") << label[i];
	}
	const wstring asciiOver[] =
	{
		L" _____ _____ _____ _____    _____ _____ _____ _____ ",
		L"|   __|  _  |     |   __|  |     |  |  |   __| __  |",
		L"|  |  |     | | | |   __|  |  |  |  | /|   __|    -|",
		L"|_____|__|__|_|_|_|_____|  |_____|___/ |_____|__|__|"
	};
	const wstring asciiClear[] =
	{
		L" _____ __    _____ _____ _____ ",
		L"|     |  |  |   __|  _  | __  |",
		L"|   --|  |__|   __|     |    -|",
		L"|_____|_____|_____|__|__|__|__|"
	};
	int titleX = res.X;
	int titleY = res.Y - 10;
	SetUnicodeMode();
	if (state.isFailed)
	{
		for (int i = 0; i < 4; ++i)
		{
			GotoXY(4, 10 + i);
			wcout << asciiOver[i] << endl;
		}
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			GotoXY(15, 10 + i);
			wcout << asciiClear[i] << endl;
		}
	}
	SetDefaultMode();
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
