#include "Console.h"
#include "GameState.h"
#include "Game.h"

int main()
{
	GameState state;
	InitGame(state);

	while (state.isRunning)
	{
		UpdateGame(state);
		RenderGame(state);
		FrameSync(60);
	}

	return 0;
}
