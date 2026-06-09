#include "Console.h"
#include "JusikHeader.h"
#include <fstream>

void CreateJusik(GameState& state)
{
	std::ifstream nameFile("TextFile\\JusikNameList.txt");
	if (!nameFile.is_open())
		return;
	for (int i = 0; i < 5; i++)
	{
		Jusik jusik;
		string line;
		nameFile >> line;
		jusik.Name = line;
	}
}

void JusikChangeNormal(GameState& state)
{
	for (Jusik& jusik : state.vecJusiks)
	{
		float value = CalculateRandomPerc(NORMAL_MIN_VALUE, NORMAL_MAX_VALUE);
		jusik.ApplyChange(value);
	}
}


void JusikChangeNews(Jusik& jusik, bool positive)
{
	float value = CalculateRandomPerc(NEWS_MIN_VALUE, NEWS_MAX_VALUE);

	if (!positive)
	{
		value *= -1;
	}
	jusik.ApplyChange(value);
}

float CalculateRandomPerc(float min, float max)
{
	int x = rand();
	int intmin = min * 10;
	int intmax = max * 10; 
	x *= 10;                 // min = 5, max = 20
	x = x % (intmax - intmin + 1) + intmin;

	float result = x / 10;

	int r = rand() % 2;
	if (r == 0)
		result *= -1;

	return result;
}
