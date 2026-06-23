#include "InfoScene.h"
#include "Console.h"

void InitInfo(GameState& state)
{
	system("cls");
	SetConsoleWindowSize(90, 30);
}

void UpdateInfo(GameState& state)
{
	if (GetKeyDown(VK_ESCAPE))
		state.curScene = Scene::TITLE;
}

void RenderInfo(GameState& state)
{
	COORD res = GetConsoleResolution();
	int x = res.X / 2;
	int y = res.Y / 3;
	const string infoLabel[] =
	{
		"[ 조작 방법 ]",
		"좌클릭 : 타일 제거",
		"우클릭/F : 깃발 설치",
		"1 : 보호구 아이템 사용(1회 무적)",
		"2 : 브러쉬 아이템 사용(지뢰를 건들면 1회 깃발을 설치함)",
		"3 : 돋보기 아이템 사용(클릭 시 해당 행과 열에 있는 지뢰 수 표시)",
		"지뢰를 제외한 모든 타일을 제거하는 것이 목표입니다.",
		"아이템은 랜덤한 타일에 숨겨져 있으며, 주로 주변에 지뢰가 많은 곳에 있습니다.",
		"아이템을 사용하면 소모되기 전 까지 계속 사용 중에 머물게 됩니다.",
		"ESC를 눌러 돌아가기"
	};
	for (int i = 0; i < 10; ++i)
	{
		GotoXY(x - 40, y + i);
		if (i == 0)
			SetColor(Color::LIGHT_YELLOW);
		else if (i == 7 || i == 8)
			SetColor(Color::LIGHT_RED);
		else if (i == 9)
			SetColor(Color::LIGHT_GRAY);
		else
			SetColor();
		cout << infoLabel[i];
	}
}
