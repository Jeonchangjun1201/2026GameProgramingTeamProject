#include "SelectScene.h"

void InitSelect(GameState& state)
{

}

void UpdateSelect(GameState& state)
{
	GameStart(state);
}

void RenderSelect(GameState& state)
{
}

void GameStart(GameState& state)
{
	state.curScene = Scene::INGAME;
}
