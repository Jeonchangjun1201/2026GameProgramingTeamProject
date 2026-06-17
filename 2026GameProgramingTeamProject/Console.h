#pragma once
#include <io.h>
#include <fcntl.h>
#define NOMINMAX
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <string>
#pragma comment(lib, "winmm.lib")

using std::wstring;
using std::string;
using std::cout;
using std::endl;
using std::wcout;
using std::cin;

// 아스키 아트
void SetUnicodeMode();
void SetDefaultMode();

// 다이얼로그
void RenderDialogue(const string& text, int delay);
void RenderChar(char ch, int delayTime, bool isSkip);

// 콘솔 창 관련 함수
void SetConsoleGameTitle(const wstring& title);
void SetConsoleWindowSize(int width, int height);
void SetConsoleFullScreen();
void SetConsoleWindowStyle(bool showTitleBar);
void ShakeConsoleWindow(int intensity, int duration, int interval);
void SetConsoleMouseInputDisabled();
POINT GetAndAdjustPosition();
COORD GetConsoleResolution();

// 커서 제어 함수
void GotoXY(int x, int y);
void SetCursorVisible(bool visible, DWORD size = 1);
BOOL IsGotoXY(int x, int y);

enum class Color
{
	BLACK, BLUE, GREEN, SKYBLUE, RED,
	VIOLET, YELLOW, LIGHT_GRAY, GRAY, LIGHT_BLUE,
	LIGHT_GREEN, CYAN, LIGHT_RED, LIGHT_VIOLET,
	LIGHT_YELLOW, WHITE, END
};
void SetColor(Color textColor = Color::WHITE, Color bgColor = Color::BLACK);

void DrawBar(int x, int y, const string& label,
	int value, int maxValue, int barWidth,
	const string& fillChar = "■", const string& emptyChar = "□");
void DrawLine(char ch, int width);

bool GetKey(int vKey);
bool GetKeyDown(int vKey);
void UpdateInput();
POINT GetMouseCellPos();
void FrameSync(int fps);
int GetCursorX();
void SetConsoleFont(const wstring& fontName, COORD size, UINT weight = FW_NORMAL);