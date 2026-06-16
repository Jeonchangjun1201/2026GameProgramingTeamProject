#pragma once
#include "GameState.h"

void LoadMap(GameState& state);

//bool TryDrawMine(GameState& state, int x, int y);
//bool TryDrawNumber(GameState& state, int x, int y);
//bool TryDrawTile(GameState& state, int x, int y);

void DrawMap(const GameState& state);
void DrawTile(const GameState& state, int x, int y);

void InitInGame(GameState& state);
void UpdateInGame(GameState& state);
void RenderInGame(GameState& state);