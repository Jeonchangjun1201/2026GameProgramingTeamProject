#include "Console.h"
#include "TitleScene.h"
#include "SoundManager.h"

void InitTitle(GameState& state)
{
	system("cls");
	SetConsoleWindowSize(120, 30);
}

void UpdateTitle(GameState& state)
{
	if (GetKeyDown(VK_UP))
	{
		SOUND->Play("move");
		state.curMenu = (Menu)std::max(0, (int)state.curMenu - 1);
	}
	if (GetKeyDown(VK_DOWN))
	{
		SOUND->Play("move");
		state.curMenu = (Menu)std::min((int)Menu::QUIT, (int)state.curMenu + 1);
	}
	if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
	{
		SOUND->Play("select");
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
	int x = res.X / 2 - 4;
	int y = res.Y / 3 * 2;
	const wstring ascii[] =
	{
		L"███╗   ███╗██╗███╗   ██╗███████╗███████╗██╗    ██╗███████╗███████╗██████╗ ███████╗██████╗	",
		L"████╗ ████║██║████╗  ██║██╔════╝██╔════╝██║    ██║██╔════╝██╔════╝██╔══██╗██╔════╝██╔══██╗",
		L"██╔████╔██║██║██╔██╗ ██║█████╗  ███████╗██║ █╗ ██║█████╗  █████╗  ██████╔╝█████╗  ██████╔╝",
		L"██║╚██╔╝██║██║██║╚██╗██║██╔══╝  ╚════██║██║███╗██║██╔══╝  ██╔══╝  ██╔═══╝ ██╔══╝  ██╔══██╗",
		L"██║ ╚═╝ ██║██║██║ ╚████║███████╗███████║╚███╔███╔╝███████╗███████╗██║     ███████╗██║  ██║",
		L"╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝ ╚══╝╚══╝ ╚══════╝╚══════╝╚═╝     ╚══════╝╚═╝  ╚═╝"
	};
	SetUnicodeMode();

	int titleX = (res.X - 90) / 2;
	int titleY = res.Y / 3;
	for (int i = 0; i < 6; ++i)
	{
		GotoXY(titleX, titleY + i);
		wcout << ascii[i] << endl;
	}
	SetDefaultMode();
	const string label[] = { "게임 시작", "게임 정보", "게임 종료" };
	for (int i = 0; i < 3; ++i)
	{
		GotoXY(x - 4, y + i);
		cout << (i == (int)state.curMenu ? "> " : "  ") << label[i];
	}
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
		SOUND->Play("transition");
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
		SOUND->Play("transition");
		Sleep(delayMs);
	}
	SetColor();
}