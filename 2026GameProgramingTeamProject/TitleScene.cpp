#include "Console.h"
#include "TitleScene.h"
#include <algorithm>

namespace
{
	int GetDisplayWidth(const string& text)
	{
		int width = 0;
		for (size_t i = 0; i < text.size(); ++i)
		{
			unsigned char c = (unsigned char)text[i];
			if (c >= 0x80)
			{
				width += 2;
				++i;
			}
			else
				width += 1;
		}
		return width;
	}
}

void InitTitle(GameState& state)
{
	system("cls");
	SetConsoleWindowSize(60, 30);
}

void UpdateTitle(GameState& state)
{
	if (GetKeyDown(VK_UP))
	{
		state.curMenu = (Menu)std::max(0, (int)state.curMenu - 1);
	}
	if (GetKeyDown(VK_DOWN))
	{
		state.curMenu = (Menu)std::min((int)Menu::QUIT, (int)state.curMenu + 1);
	}
	if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
	{
		switch (state.curMenu)
		{
		case Menu::START:
			PlayTransition();
			MoveToSelectScene(state);
			break;
		case Menu::INFO:
			state.curScene = Scene::INFO;
			break;
		case Menu::QUIT:
			state.isRunning = false;
			break;
		}
	}
}

void RenderTitle(GameState& state)
{
	COORD res = GetConsoleResolution();
	int y = res.Y / 3 * 2;
	const string label[] = { "게임 시작", "게임 정보", "게임 종료" };
	int maxWidth = 0;
	for (int i = 0; i < 3; ++i)
	{
		string line = string("> ") + label[i];
		maxWidth = std::max(maxWidth, GetDisplayWidth(line));
	}
	int startX = res.X / 2 - maxWidth / 2;

	for (int i = 0; i < 3; ++i)
	{
		GotoXY(startX, y + i);
		if (i == (int)state.curMenu)
			SetColor(Color::LIGHT_GREEN);
		else
			SetColor();
		cout << (i == (int)state.curMenu ? "> " : "  ") << label[i];
	}
	SetColor();
}

void MoveToSelectScene(GameState& state)
{
	state.curScene = Scene::SELECT;
}
void PlayTransition()
{
	COORD res = GetConsoleResolution();
	int delayMs = 20;
	int flashCount = 5;
	FlashAnim(res, flashCount, delayMs);
	CrossAnim(res, delayMs);
}

void FlashAnim(COORD res, int count, int delayMs)
{
	for (int i = 0; i < count; ++i)
	{
		SetColor(Color::BLACK, Color::WHITE);
		system("cls");
		Sleep(delayMs);

		SetColor();
		system("cls");
		Sleep(delayMs);
	}
}

void CrossAnim(COORD res, int delayMs)
{
	SetColor(Color::BLACK, Color::WHITE);
	for (int x = 0; x < res.X / 2; ++x)
	{
		for (int y = 0; y < res.Y; y += 2)
		{
			GotoXY(x * 2, y);
			cout << "  ";
		}
		for (int y = 1; y < res.Y; y += 2)
		{
			GotoXY(res.X - 2 - x * 2, y);
			cout << "  ";
		}
		Sleep(delayMs);
	}
	SetColor();
}