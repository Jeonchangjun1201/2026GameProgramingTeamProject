#include "Console.h"
#include "SelectScene.h"
#include <algorithm>
#include "SoundManager.h"

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

	int GetCenteredX(int screenWidth, const string& text)
	{
		return screenWidth / 2 - GetDisplayWidth(text) / 2;
	}

	string BuildMenuLine(const string& prefix, const string& label, const string& detail, int labelColWidth)
	{
		string line = prefix + label;
		while (GetDisplayWidth(line) < labelColWidth)
			line += " ";
		line += "  ";
		line += detail;
		return line;
	}
}

void InitSelect(GameState& state)
{
	system("cls");
	SetConsoleWindowSize(60, 30);
}

void UpdateSelect(GameState& state)
{
	if (GetKeyDown(VK_UP))
	{
		SOUND->Play("move");
		state.curDiff = (Difficulty)std::max(0, (int)state.curDiff - 1);
	}
	if (GetKeyDown(VK_DOWN))
	{
		SOUND->Play("move");
		state.curDiff = (Difficulty)std::min((int)Difficulty::EXTREME, (int)state.curDiff + 1);
	}
	if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
	{
		SOUND->Play("select");
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
	int startY = res.Y / 3;
	const string label[] = { "쉬움", "보통", "어려움", "극한" };
	const string detail[] =
	{
		"9 x 9   | 지뢰 10개",
		"16 x 16 | 지뢰 40개",
		"16 x 30 | 지뢰 99개",
		"30 x 30 | 지뢰 199개"
	};
	const string title = "난이도 선택";
	const string footer = "Enter : 게임 시작  |  ESC : 돌아가기";
	const int labelColWidth = GetDisplayWidth(string("> ") + label[2]);

	int menuMaxWidth = 0;
	for (int i = 0; i < 4; ++i)
	{
		string line = BuildMenuLine("> ", label[i], detail[i], labelColWidth);
		menuMaxWidth = std::max(menuMaxWidth, GetDisplayWidth(line));
	}
	int menuStartX = res.X / 2 - menuMaxWidth / 2;

	GotoXY(GetCenteredX(res.X, title), startY - 2);
	SetColor(Color::LIGHT_YELLOW);
	cout << title;

	for (int i = 0; i < 4; ++i)
	{
		string line = BuildMenuLine(
			i == (int)state.curDiff ? "> " : "  ",
			label[i],
			detail[i],
			labelColWidth);

		GotoXY(menuStartX, startY + i * 2);
		if (i == (int)state.curDiff)
			SetColor(Color::LIGHT_GREEN);
		else
			SetColor();
		cout << line;
	}

	GotoXY(GetCenteredX(res.X, footer), startY + 10);
	SetColor(Color::LIGHT_GRAY);
	cout << footer;
	SetColor();
}

void GameStart(GameState& state)
{
	state.curScene = Scene::INGAME;
}