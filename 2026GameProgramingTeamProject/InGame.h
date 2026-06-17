#pragma once
#include "GameState.h"

void LoadMap(GameState& state);

//bool TryDrawMine(GameState& state, int x, int y);
//bool TryDrawNumber(GameState& state, int x, int y);
//bool TryDrawTile(GameState& state, int x, int y);

void DrawMap(GameState& state);
void DrawTile(GameState& state, int x, int y);
void DrawNumber(GameState& state, int x, int y);
void PlaceMine(GameState& state);
void RevealTile(GameState& state, int x, int y);
bool IsInRange(GameState& state, int x, int y);

void InitInGame(GameState& state);
void UpdateInGame(GameState& state);
void RenderInGame(GameState& state);