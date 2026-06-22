#pragma once
#include "../GameState.h"

ItemType RollRandomItem();
bool UseItem(GameState& state, ItemType type);
void CollectRevealedTreasures(GameState& state);
void CheckRevealBonus(GameState& state);
bool HandleMineExplosion(GameState& state, int x, int y);
void GrantItem(GameState& state, ItemType type, bool bonus);
void SetupRunInventory(GameState& state);
void CommitRunInventory(GameState& state);
void AbandonRunInventory(GameState& state);
