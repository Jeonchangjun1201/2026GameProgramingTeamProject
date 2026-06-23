#include "Console.h"
#include "SelectScene.h"

void InitSelect(GameState& state)
{
	system("cls");
	SetConsoleWindowSize(60, 30);
}

void UpdateSelect(GameState& state)
{
	if (GetKeyDown(VK_UP))
	{
		state.curDiff = (Difficulty)std::max(0, (int)state.curDiff - 1);
	}
	if (GetKeyDown(VK_DOWN))
	{
		state.curDiff = (Difficulty)std::min((int)Difficulty::EXTREME, (int)state.curDiff + 1);
	}
	if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
	{
		GameStart(state);
	}
	if (GetKeyDown(VK_ESCAPE))
	{
		state.curScene = Scene::TITLE;
		state.curMenu = Menu::START;
	}
}

void RenderSelect(GameState& state)
{
	COORD res = GetConsoleResolution();
	int centerX = res.X / 2;
	int startY = res.Y / 3;
	const string label[] = { "쉬움", "보통", "어려움", "극한" };
	const string detail[] =
	{
		"9 x 9   | 지뢰 10개",
		"16 x 16 | 지뢰 40개",
		"16 x 30 | 지뢰 99개",
		"30 x 30 | 지뢰 199개"
	};

	GotoXY(centerX - 8, startY - 2);
	SetColor(Color::LIGHT_YELLOW);
	cout << "난이도 선택";

	for (int i = 0; i < 4; ++i)
	{
		GotoXY(centerX - 14, startY + i * 2);
		if (i == (int)state.curDiff)
			SetColor(Color::LIGHT_GREEN);
		else
			SetColor();

		cout << (i == (int)state.curDiff ? "> " : "  ") << label[i];
		GotoXY(centerX + 2, startY + i * 2);
		cout << detail[i];
	}

	GotoXY(centerX - 12, startY + 10);
	SetColor(Color::LIGHT_GRAY);
	cout << "Enter : 게임 시작  |  ESC : 돌아가기";
	SetColor();
}

void GameStart(GameState& state)
{
	state.curScene = Scene::INGAME;
}