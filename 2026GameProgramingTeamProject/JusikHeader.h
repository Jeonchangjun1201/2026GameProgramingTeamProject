#pragma once
#include <vector>
#include "GameState.h"

void CreateJusik(GameState& state);

void JusikChangeNormal(GameState& state);
void JusikChangeNews(Jusik& jusik, bool positive);

float CalculateRandomPerc(float min, float max);