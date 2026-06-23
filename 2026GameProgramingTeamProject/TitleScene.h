#pragma once
#include "GameState.h"

void InitTitle(GameState& state);
void UpdateTitle(GameState& state);
void RenderTitle(GameState& state);
void MoveToSelectScene(GameState& state);

void PlayTransition();
void FlashAnim(COORD res, int count, int delayMs);
void CrossAnim(COORD res, int delayMs);