#include "Console.h";
#include "GameOverScene.h"

void InitGameOver(GameState& state)
{
	SetConsoleWindowSize(60, 30);
}

void UpdateGameOver(GameState& state)
{
	//if (GetKeyDown(VK_UP))
	//{
	//	state.curOverMenu = (Menu)std::max(0, (int)state.curMenu - 1);
	//}
	//if (GetKeyDown(VK_DOWN))
	//{
	//	state.curOverMenu = (Menu)std::min((int)Menu::QUIT, (int)state.curMenu + 1);
	//}
	//if (GetKeyDown(VK_SPACE) || GetKeyDown(VK_RETURN))
	//{
	//	switch (state.curMenu)
	//	{
	//	case Menu::START:
	//		PlayTransition();
	//		MoveToSelectScene(state);
	//		break;
	//	case Menu::INFO:
	//		state.curScene = Scene::INFO;
	//		break;
	//	case Menu::QUIT:
	//		state.isRunning = false;
	//		break;
	//	}
	//}
}

void RenderGameOver(GameState& state)
{
}
