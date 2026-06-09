#pragma once
#include <vector>

constexpr float NORMAL_MIN_VALUE = 0.5f;
constexpr float NORMAL_MAX_VALUE = 2;
constexpr float NEWS_MIN_VALUE = 10;
constexpr float NEWS_MAX_VALUE = 20;

struct Jusik
{
public:
	string Name;
	int Value;
	void ApplyChange(float percentage)
	{
		Value += Value * percentage;
	}
};

struct GameState
{
	std::vector<Jusik> vecJusiks;
};
