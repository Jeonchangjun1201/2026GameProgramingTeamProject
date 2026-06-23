#include "Console.h"
#include "GameState.h"
#include "Game.h"

int main()
{
	GameState state;
	InitGame(state);

	while (true)
	{
		ULONGLONG curTime = state.timeStopped ? state.lastTime : GetTickCount64();
		state.lastTime = curTime;
		UpdateGame(state);
		RenderGame(state);
	}
}