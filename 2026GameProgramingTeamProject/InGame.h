#pragma once
#include "GameState.h"
#include <wincrypt.h>

void LoadMap(GameState& state);

//bool TryDrawMine(GameState& state, int x, int y);
//bool TryDrawNumber(GameState& state, int x, int y);
//bool TryDrawTile(GameState& state, int x, int y);

void DrawMap(GameState& state);
void DrawTile(GameState& state, int x, int y);
void DrawNumber(GameState& state, int x, int y);
void PlaceMine(GameState& state);
void PlaceItems(GameState& state);
void RevealTile(GameState& state, int x, int y);
void ActivatedMine(GameState& state);
void UpdateCurrentFlags(GameState& state);
bool IsInRange(GameState& state, int x, int y);
bool IsAllTileClear(GameState& state);
void DrawUI(GameState& state);
int CheckMinesInRange(GameState& state, int x, int y);
void GrantItem(GameState& state);
void UseMagnify(GameState& state, int x, int y);

void InitInGame(GameState& state);
void UpdateInGame(GameState& state);
void RenderInGame(GameState& state);