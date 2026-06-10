#include "Console.h"
#include "GameState.h"
#include "Game.h"

int main()
{
	GameState state;
	InitGame(state);

	while (true)
	{
		UpdateGame(state);
		RenderGame(state);
	}
}