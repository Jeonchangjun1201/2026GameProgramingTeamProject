#include "Game.h"
#include "Console.h"
#include "EndingArt.h"
#include "Minesweeper/ItemManager.h"
#include <ctime>
#include <string>

static bool prevLeftDown = false;
static bool prevRightDown = false;

static bool WasMousePressed(int vKey, bool& prevDown)
{
	bool cur = (GetAsyncKeyState(vKey) & 0x8000) != 0;
	bool pressed = cur && !prevDown;
	prevDown = cur;
	return pressed;
}

static Color GetNumberColor(int n)
{
	switch (n)
	{
	case 1: return Color::BLUE;
	case 2: return Color::GREEN;
	case 3: return Color::RED;
	case 4: return Color::VIOLET;
	case 5: return Color::YELLOW;
	case 6: return Color::CYAN;
	case 7: return Color::LIGHT_GRAY;
	case 8: return Color::GRAY;
	default: return Color::WHITE;
	}
}

static void StartNewGame(GameState& state, Difficulty diff)
{
	state.curDiff = diff;
	state.config = GetDifficultyConfig(diff);
	state.curScene = Scene::INGAME;
	state.board.Reset(state.config.width, state.config.height, state.config.mineCount);
	SetupRunInventory(state);
	state.brushEquipped = false;
	state.armorCount = 0;
	state.firstClick = true;
	state.gameStarted = false;
	state.needsFullRedraw = true;
	state.magnifierUntil = 0;
	state.compassUntil = 0;
	state.itemNotifyUntil = 0;
	state.notifyItem = ItemType::None;
	state.curTime = GetTickCount64();
	SetConsoleWindowSize(GetRequiredConsoleWidth(state.config.width), 40);
}

static const char* GetCompassArrowText(int dx, int dy)
{
	if (dx == 0 && dy == -1) return "^";
	if (dx == 1 && dy == -1) return "/^";
	if (dx == 1 && dy == 0)  return ">";
	if (dx == 1 && dy == 1)  return "\\v";
	if (dx == 0 && dy == 1)  return "v";
	if (dx == -1 && dy == 1) return "/v";
	if (dx == -1 && dy == 0) return "<";
	if (dx == -1 && dy == -1) return "\\^";
	return "?";
}

static void FinishWin(GameState& state)
{
	CommitRunInventory(state);
	state.curScene = Scene::WIN;
	state.needsFullRedraw = true;
	PlaySound(L"SystemAsterisk", nullptr, SND_ASYNC | SND_ALIAS);
}

static void ProcessCellClick(GameState& state, int x, int y, bool rightClick)
{
	if (state.curScene != Scene::INGAME || state.board.IsGameOver())
		return;
	if (!state.board.InBounds(x, y))
		return;

	if (rightClick)
	{
		if (state.board.InBounds(x, y))
		{
			state.actionCell.x = x;
			state.actionCell.y = y;
		}
		if (state.board.ToggleFlag(x, y))
			PlaySound(L"SystemDefault", nullptr, SND_ASYNC | SND_ALIAS);
		return;
	}

	if (state.board.InBounds(x, y))
	{
		state.actionCell.x = x;
		state.actionCell.y = y;
	}

	if (state.firstClick)
	{
		state.board.GenerateBoard(x, y, state.config.treasureZoneCount, state.config.treasuresPerZone);
		state.firstClick = false;
		state.gameStarted = true;
	}

	state.board.ResetRevealCount();
	RevealResult result = state.board.RevealCell(x, y, state.brushEquipped);
	CollectRevealedTreasures(state);
	CheckRevealBonus(state);

	switch (result)
	{
	case RevealResult::None:
		return;
	case RevealResult::BrushFlagged:
		if (!state.inventory.RemoveItem(ItemType::Brush, 1))
			state.brushEquipped = false;
		else if (state.inventory.GetCount(ItemType::Brush) <= 0)
			state.brushEquipped = false;
		PlaySound(L"SystemHand", nullptr, SND_ASYNC | SND_ALIAS);
		break;
	case RevealResult::Revealed:
	case RevealResult::TreasureFound:
		PlaySound(L"SystemDefault", nullptr, SND_ASYNC | SND_ALIAS);
		break;
	case RevealResult::Exploded:
		if (HandleMineExplosion(state, x, y))
		{
			AbandonRunInventory(state);
			state.curScene = Scene::GAMEOVER;
			state.needsFullRedraw = true;
		}
		return;
	}

	if (state.board.IsWin())
		FinishWin(state);
}

static bool IsBoardGridLine(int screenX, int screenY)
{
	int lx = screenX - BOARD_OFFSET_X;
	int ly = screenY - BOARD_OFFSET_Y;
	if (lx < 0 || ly < 0)
		return false;
	return (lx % CELL_STRIDE_X == 1) || (ly % CELL_STRIDE_Y == 1);
}

static void DrawGridLineVertical(int sx, int sy)
{
	GotoXY(sx, sy);
	SetColor(Color::GRAY);
	wcout << L'\u2502';
}

static void DrawGridLineHorizontal(int ox, int sy, int boardWidth)
{
	for (int x = 0; x < boardWidth; ++x)
	{
		GotoXY(ox + x * CELL_STRIDE_X, sy);
		SetColor(Color::GRAY);
		wcout << L'\u2500';
		GotoXY(ox + x * CELL_STRIDE_X + 1, sy);
		wcout << L'\u253c';
	}
}

static void DrawCell(int sx, int sy, const Cell& cell)
{
	GotoXY(sx, sy);
	if (!cell.isRevealed)
	{
		if (cell.isFlagged)
		{
			SetColor(Color::LIGHT_YELLOW);
			wcout << L'F';
			return;
		}
		if (cell.isTreasureZone) SetColor(Color::YELLOW);
		else SetColor(Color::WHITE);
		wcout << L"\u25A0";
		return;
	}
	if (cell.isMine) { SetColor(Color::WHITE, Color::RED); wcout << L'*'; return; }
	if (cell.isTreasure && !cell.treasureCollected) { SetColor(Color::LIGHT_YELLOW); wcout << L'$'; return; }
	if (cell.neighborMines == 0)
	{
		SetColor(Color::GRAY);
		wcout << L"\u25A1";
		return;
	}
	SetColor(GetNumberColor(cell.neighborMines));
	wcout << static_cast<wchar_t>(L'0' + cell.neighborMines);
}

static void RenderBoard(const GameState& state)
{
	int w = state.board.GetWidth();
	int h = state.board.GetHeight();
	SetUnicodeMode();
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			int sx = CellToScreenX(x);
			int sy = CellToScreenY(y);
			DrawCell(sx, sy, state.board.GetCell(x, y));
			DrawGridLineVertical(sx + 1, sy);
		}
		if (y + 1 < h)
			DrawGridLineHorizontal(BOARD_OFFSET_X, CellToScreenY(y) + 1, w);
	}
	SetDefaultMode();
	SetColor();
}

static void RenderItemNotify(const GameState& state, int hudX)
{
	if (state.curTime >= state.itemNotifyUntil || state.notifyItem == ItemType::None)
		return;

	bool blink = ((state.curTime / 150) % 2) == 0;
	GotoXY(hudX, 22);
	SetColor(blink ? Color::LIGHT_YELLOW : Color::LIGHT_GREEN);
	cout << " *>> " << GetItemName(state.notifyItem) << " ";
	if (state.notifyBonus)
		cout << "\xB4\xEB\xB3\xB4\xB3\xCA\x21";
	cout << " <<* ";
	SetColor();
	GotoXY(hudX, 23);
	SetColor(blink ? Color::LIGHT_VIOLET : Color::CYAN);
	cout << "==========================";
	SetColor();
}

static void RenderHud(const GameState& state)
{
	int hudX = GetHudOffsetX(state.config.width);
	GotoXY(hudX, 2);
	SetColor(Color::LIGHT_GREEN);
	cout << "== MINESWEEPER ==";
	SetColor();
	GotoXY(hudX, 3);
	cout << "Saved: " << state.savedInventory.GetTotalItemCount();
	GotoXY(hudX, 4); cout << "Mines: " << state.board.GetRemainingMines();
	GotoXY(hudX, 5); cout << GetItemName(ItemType::Armor) << ": " << (state.armorCount > 0 ? "\xC0\xD6\xC0\xBD" : "\xBE\xF8\xC0\xBD");
	GotoXY(hudX, 6); cout << GetItemName(ItemType::Brush) << ": " << (state.brushEquipped ? "\xC4\xD1\xC1\xFC" : "\xB2\xA8\xC1\xFC");
	GotoXY(hudX, 8); cout << "\xBE\xC6\xC0\xCC\xC5\xDB (1-9):";
	for (int i = 0; i < state.inventory.GetSlotCount() && i < 9; ++i)
	{
		GotoXY(hudX, 9 + i);
		cout << (i + 1) << ". " << GetItemName(state.inventory.GetSlotType(i))
			<< " x" << state.inventory.GetSlotItemCount(i);
	}
	if (state.curTime < state.compassUntil)
	{
		GotoXY(hudX, 19);
		cout << GetItemName(ItemType::Compass) << ": "
			<< GetCompassArrowText(state.compassDirX, state.compassDirY);
	}
	if (state.curTime < state.magnifierUntil)
	{
		GotoXY(hudX, 20);
		cout << "\xC7\xE0 " << state.cursor.y << " \xC1\xF6\xB7\xE1: " << state.board.CountMinesInRow(state.cursor.y);
		GotoXY(hudX, 21);
		cout << "\xBF\xAD " << state.cursor.x << " \xC1\xF6\xB7\xE1: " << state.board.CountMinesInCol(state.cursor.x);
	}
	RenderItemNotify(state, hudX);
	GotoXY(hudX, 25); cout << "LClick:Open  RClick/F:Flag";
	GotoXY(hudX, 26); cout << "1-9:Item  Esc:Pause  Q:Quit";
}

void InitGame(GameState& state)
{
	srand(static_cast<unsigned>(time(nullptr)));
	state = GameState{};
	state.curScene = Scene::TITLE;
	state.needsFullRedraw = true;

	SetConsoleWindowSize(80, 40);
	SetConsoleOutputCP(949);
	SetConsoleCP(949);
	SetConsoleWindowStyle(true);
	SetCursorVisible(false);
	SetConsoleMouseInputDisabled();
	SetConsoleGameTitle(L"Minesweeper");
}

void UpdateGame(GameState& state)
{
	ConsumeConsoleInputEvents();
	UpdateInput();
	state.curTime = GetTickCount64();

	switch (state.curScene)
	{
	case Scene::TITLE:
		if (GetKeyDown(VK_RETURN) || GetKeyDown(VK_SPACE))
		{
			state.curScene = Scene::SELECT;
			state.needsFullRedraw = true;
		}
		break;

	case Scene::SELECT:
		if (GetKeyDown('1')) StartNewGame(state, Difficulty::EASY);
		else if (GetKeyDown('2')) StartNewGame(state, Difficulty::NORMAL);
		else if (GetKeyDown('3')) StartNewGame(state, Difficulty::HARD);
		else if (GetKeyDown(VK_ESCAPE))
		{
			state.curScene = Scene::TITLE;
			state.needsFullRedraw = true;
		}
		break;

	case Scene::INGAME:
	{
		POINT mousePos = GetMouseCellPos();
		if (!IsBoardGridLine(mousePos.x, mousePos.y))
		{
			state.cursor.x = ScreenToCellX(mousePos.x);
			state.cursor.y = ScreenToCellY(mousePos.y);
		}
		if (state.cursor.x < 0) state.cursor.x = 0;
		if (state.cursor.y < 0) state.cursor.y = 0;
		if (state.cursor.x >= state.config.width) state.cursor.x = state.config.width - 1;
		if (state.cursor.y >= state.config.height) state.cursor.y = state.config.height - 1;
		state.actionCell = state.cursor;

		if (GetKeyDown(VK_ESCAPE))
		{
			state.curScene = Scene::PAUSE;
			state.needsFullRedraw = true;
			break;
		}

		for (int i = 0; i < state.inventory.GetSlotCount() && i < 9; ++i)
		{
			if (GetKeyDown('1' + i))
			{
				UseItem(state, state.inventory.GetSlotType(i));
				state.needsFullRedraw = true;
			}
		}

		POINT mouse = GetMouseCellPos();
		if (IsBoardGridLine(mouse.x, mouse.y))
			break;
		int cellX = ScreenToCellX(mouse.x);
		int cellY = ScreenToCellY(mouse.y);

		if (WasMousePressed(VK_LBUTTON, prevLeftDown))
		{
			ProcessCellClick(state, cellX, cellY, false);
			state.needsFullRedraw = true;
		}
		else if (WasMousePressed(VK_RBUTTON, prevRightDown) || GetKeyDown('F'))
		{
			ProcessCellClick(state, cellX, cellY, true);
			state.needsFullRedraw = true;
		}

		if (state.curTime < state.itemNotifyUntil)
		{
			static ULONGLONG lastNotifyRedraw = 0;
			if (state.curTime - lastNotifyRedraw >= 150)
			{
				state.needsFullRedraw = true;
				lastNotifyRedraw = state.curTime;
			}
		}
		break;
	}

	case Scene::PAUSE:
		if (GetKeyDown(VK_ESCAPE))
		{
			state.curScene = Scene::INGAME;
			state.needsFullRedraw = true;
		}
		else if (GetKeyDown('Q'))
		{
			AbandonRunInventory(state);
			state.curScene = Scene::TITLE;
			state.needsFullRedraw = true;
		}
		break;

	case Scene::WIN:
	case Scene::GAMEOVER:
		if (GetKeyDown(VK_RETURN) || GetKeyDown(VK_SPACE) || GetKeyDown(VK_ESCAPE))
		{
			state.curScene = Scene::TITLE;
			state.needsFullRedraw = true;
		}
		break;
	default:
		break;
	}
}

void RenderGame(GameState& state)
{
	if (state.needsFullRedraw)
	{
		system("cls");
		state.needsFullRedraw = false;
	}

	switch (state.curScene)
	{
	case Scene::TITLE:
		GotoXY(20, 10); SetColor(Color::CYAN);
		cout << "=============================";
		GotoXY(20, 11); cout << "       MINESWEEPER";
		GotoXY(20, 12); cout << "=============================";
		SetColor();
		GotoXY(20, 15); cout << "Press ENTER to Start";
		break;

	case Scene::SELECT:
		GotoXY(20, 8); SetColor(Color::LIGHT_YELLOW); cout << "Select Difficulty"; SetColor();
		GotoXY(20, 11); cout << "1. " << GetDifficultyName(Difficulty::EASY);
		GotoXY(20, 12); cout << "2. " << GetDifficultyName(Difficulty::NORMAL);
		GotoXY(20, 13); cout << "3. " << GetDifficultyName(Difficulty::HARD);
		GotoXY(20, 15); cout << "Saved items: " << state.savedInventory.GetTotalItemCount();
		GotoXY(20, 16); cout << "Clear to keep items / Quit loses run loot";
		GotoXY(20, 18); cout << "Esc: Back";
		break;

	case Scene::INGAME:
	case Scene::PAUSE:
		GotoXY(0, 0); cout << "Difficulty: " << GetDifficultyName(state.curDiff);
		RenderBoard(state);
		RenderHud(state);
		if (state.curScene == Scene::PAUSE)
		{
			GotoXY(20, 18); SetColor(Color::LIGHT_YELLOW);
			cout << "=== PAUSED ===  Esc/Q";
			SetColor();
		}
		break;

	case Scene::WIN:
		RenderWinArt();
		break;

	case Scene::GAMEOVER:
		RenderGameOverArt();
		break;
	default:
		break;
	}
}
