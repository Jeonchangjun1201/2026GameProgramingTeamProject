#pragma once

enum class Scene { NONE, TITLE, SELECT, INGAME, PAUSE, WIN, GAMEOVER };
enum class Difficulty { EASY, NORMAL, HARD };

enum class ItemType
{
	None,
	MagnifyingGlass,
	Armor,
	Brush,
	Compass,
	SafeScanner,
	DefuseKit,
	Hammer,
	COUNT
};

struct DifficultyConfig
{
	int width = 9;
	int height = 9;
	int mineCount = 10;
	int treasureZoneCount = 1;
	int treasuresPerZone = 2;
};

constexpr int MAX_INVENTORY = 9;
constexpr int BOARD_OFFSET_X = 1;
constexpr int BOARD_OFFSET_Y = 2;
constexpr int CELL_STRIDE_X = 2;
constexpr int CELL_STRIDE_Y = 2;
constexpr int HUD_GAP = 2;

inline int CellToScreenX(int cellX) { return BOARD_OFFSET_X + cellX * CELL_STRIDE_X; }
inline int CellToScreenY(int cellY) { return BOARD_OFFSET_Y + cellY * CELL_STRIDE_Y; }
inline int ScreenToCellX(int screenX) { return (screenX - BOARD_OFFSET_X) / CELL_STRIDE_X; }
inline int ScreenToCellY(int screenY) { return (screenY - BOARD_OFFSET_Y) / CELL_STRIDE_Y; }
inline int GetHudOffsetX(int boardWidth) { return BOARD_OFFSET_X + boardWidth * CELL_STRIDE_X + HUD_GAP; }
inline int GetRequiredConsoleWidth(int boardWidth) { return GetHudOffsetX(boardWidth) + 28; }
constexpr int TREASURE_ZONE_RADIUS = 2;
constexpr int TREASURE_ZONE_CLUSTER_CHANCE = 42;
constexpr int COMPASS_DURATION_MS = 5000;
constexpr int FLOOD_BONUS_TILE_COUNT = 9;
constexpr int ITEM_NOTIFY_MS = 2800;
inline DifficultyConfig GetDifficultyConfig(Difficulty difficulty)
{
	switch (difficulty)
	{
	case Difficulty::EASY:   return { 9, 9, 10, 1, 2 };
	case Difficulty::NORMAL: return { 16, 16, 40, 2, 3 };
	case Difficulty::HARD:   return { 30, 16, 99, 3, 4 };
	default:                 return { 9, 9, 10, 1, 2 };
	}
}

inline const char* GetItemName(ItemType type)
{
	switch (type)
	{
	case ItemType::MagnifyingGlass: return "\xB5\xB8\xBA\xB8\xB1\xE2";
	case ItemType::Armor:           return "\xB9\xE6\xBE\xEE\xB1\xB8";
	case ItemType::Brush:           return "\xBC\xD6";
	case ItemType::Compass:         return "\xB3\xAA\xC4\xA7\xB9\xDD";
	case ItemType::SafeScanner:     return "\xBE\xC8\xC0\xFC\xBD\xBA\xC4\xB3\xB3\xCA";
	case ItemType::DefuseKit:       return "\xC7\xD8\xC3\xBC\xC5\xB0\xC6\xAE";
	case ItemType::Hammer:          return "\xB8\xC1\xC4\xA1";
	default:                        return "-";
	}
}

inline const char* GetDifficultyName(Difficulty difficulty)
{
	switch (difficulty)
	{
	case Difficulty::EASY:   return "Easy (9x9)";
	case Difficulty::NORMAL: return "Normal (16x16)";
	case Difficulty::HARD:   return "Hard (30x16)";
	default:                 return "Unknown";
	}
}
