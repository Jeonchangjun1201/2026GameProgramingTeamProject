#include "Console.h"
//�ƽ�Ű ��Ʈ
void SetUnicodeMode()
{
	(void)_setmode(_fileno(stdout), _O_U16TEXT);
}

void SetDefaultMode()
{
	(void)_setmode(_fileno(stdout), _O_TEXT);
}

void RenderDialogue(const string& text, int delay)
{
	bool isSkip = false;
	for (int i = 0; i < text.size(); ++i)
	{
		if (!isSkip && _kbhit())
		{
			_getch();
			isSkip = true;
		}
		RenderChar(text[i], delay, isSkip);
	}
}

void RenderChar(char ch, int delayTime, bool isSkip)
{
	cout << ch;

	if (strchr("@#$%^&", ch))
	{
		if (!isSkip)
			// ��ŵ�ƴҶ���
			Sleep(200);
		cout << "\b \b";

		if (!isSkip)
			// ��ŵ�ƴҶ���
			Sleep(100);
	}

	if (isSkip)
		return;


	int calcDelay = delayTime + (rand() % 11 - 5);
	switch (ch)
	{
	case '!':
	case '?':
	case '.':
		calcDelay += 400; // 0.4��
		break;
	case ',':
		calcDelay += 200; // 0.2��
		break;
	case '\n':
		calcDelay += 500; // 0.5��
		break;
	case ' ':
		calcDelay += 50; // 0.05��
		break;
	}
	Sleep(std::max(0, calcDelay));
}

void SetConsoleGameTitle(const wstring& title)
{
	SetConsoleTitle(title.c_str());
}

void SetConsoleWindowSize(int width, int height)
{
	// �ܼ� ����: ����, ���� ��ų�
	//GetStdHandle(STD_OUTPUT_HANDLE);

	//�ܼ� �ܺ�: �ܼ� â ��ü�� �ǵ帱 ��
	//GetConsoleWindow();

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	// ����Ʈ ���̱�
	SMALL_RECT rect = { 0, 0, 1, 1 };
	SetConsoleWindowInfo(handle, true, &rect);

	// ���� �ػ󵵿� �°� ����
	COORD size = { (short)width, (short)height };
	SetConsoleScreenBufferSize(handle, size);


	//����Ʈ ���߱� 
	rect.Right = width - 1;
	rect.Bottom = height - 1;
	SetConsoleWindowInfo(handle, true, &rect);

	// ��� ����
	HWND hWnd = GetConsoleWindow();

	//RECT windowRect;
	//GetWindowRect(hWnd, &windowRect);

	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(handle, false, &fontInfo);

	int windowWidth = fontInfo.dwFontSize.X * width;
	int windowHeight = fontInfo.dwFontSize.Y * height;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = (screenWidth - windowWidth) / 2;
	int posY = (screenHeight - windowHeight) / 2;

	SetWindowPos(hWnd, nullptr, posX, posY, 0, 0, SWP_NOSIZE);
}

void SetConsoleFullScreen()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(handle, false, &fontInfo);

	SetConsoleDisplayMode(handle, CONSOLE_FULLSCREEN_MODE, 0);

	int width = GetSystemMetrics(SM_CXSCREEN) / fontInfo.dwFontSize.X;
	int height = GetSystemMetrics(SM_CYSCREEN) / fontInfo.dwFontSize.Y;
	SetConsoleWindowSize(width, height);

}

void SetConsoleWindowStyle(bool showTitleBar)
{
	HWND hWnd = GetConsoleWindow();
	if (hWnd == nullptr)
		return;
	LONG style = GetWindowLong(hWnd, GWL_STYLE);
	style &= ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX & ~WS_SYSMENU;
	if (!showTitleBar)
		style &= ~WS_CAPTION;
	SetWindowLong(hWnd, GWL_STYLE, style);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
}

void ShakeConsoleWindow(int intensity, int duration, int interval)
{
	HWND hWnd = GetConsoleWindow();
	RECT rt = {};
	GetWindowRect(hWnd, &rt);
	int originX = rt.left;
	int originY = rt.top;

	int count = duration / interval;

	for (int i = 0; i < count; i++)
	{
		int offsetX = rand() % (2 * intensity + 1) - intensity;
		int offsetY = rand() % (2 * intensity + 1) - intensity;
		SetWindowPos(hWnd, nullptr,
			originX + offsetX,
			originY + offsetY, 0, 0, SWP_NOSIZE);
		Sleep(interval);
	}
	SetWindowPos(hWnd, nullptr, originX, originY, 0, 0, SWP_NOSIZE);
}

static WNDPROC g_consoleOrigWndProc = nullptr;

static LRESULT CALLBACK ConsoleGameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CONTEXTMENU:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		return 0;
	}
	return CallWindowProc(g_consoleOrigWndProc, hwnd, msg, wParam, lParam);
}

static void HookConsoleWindowProc()
{
	HWND hwnd = GetConsoleWindow();
	if (hwnd == nullptr)
		return;
	if (g_consoleOrigWndProc == nullptr)
	{
		g_consoleOrigWndProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC);
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)ConsoleGameWndProc);
	}
}

void SetConsoleMouseInputDisabled()
{
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(handle, &mode);
	mode |= ENABLE_EXTENDED_FLAGS;
	mode &= ~ENABLE_QUICK_EDIT_MODE;
	mode &= ~ENABLE_INSERT_MODE;
	mode |= ENABLE_MOUSE_INPUT;
	SetConsoleMode(handle, mode);
	HookConsoleWindowProc();
}

void ConsumeConsoleInputEvents()
{
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	DWORD count = 0;
	while (GetNumberOfConsoleInputEvents(handle, &count) && count > 0)
	{
		INPUT_RECORD records[64];
		DWORD read = 0;
		DWORD toRead = count > 64 ? 64 : count;
		if (!ReadConsoleInput(handle, records, toRead, &read) || read == 0)
			break;
	}
}

COORD GetConsoleResolution()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	short width = info.srWindow.Right - info.srWindow.Left + 1;
	short height = info.srWindow.Bottom - info.srWindow.Top + 1;
	return { width, height };
}

void GotoXY(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD curPos = { x, y };

	SetConsoleCursorPosition(handle, curPos);
}

void SetCursorVisible(bool visible, DWORD size)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfo = {};
	curInfo.bVisible = visible;
	curInfo.dwSize = size;
	SetConsoleCursorInfo(handle, &curInfo);
}

BOOL IsGotoXY(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD curPos = { x, y };

	return SetConsoleCursorPosition(handle, curPos);
}

void SetColor(Color textColor, Color bgColor)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int text = (int)textColor;
	int bg = (int)bgColor;
	SetConsoleTextAttribute(handle, (bg << 4) | text);
}

void DrawBar(int x, int y, const string& label, int value, int maxValue, int barWidth, const string& fillChar, const string& emptyChar)
{
	Color color;
	if (value > maxValue * 0.6f)
		color = Color::LIGHT_GREEN;
	else if (value > maxValue * 0.3f)
		color = Color::LIGHT_YELLOW;
	else
		color = Color::LIGHT_RED;

	GotoXY(x, y);
	SetColor();
	cout << label;

	SetColor(color);
	int fillValue = barWidth * value / maxValue;
	for (int i = 0; i < barWidth;++i)
		cout << (i < fillValue ? fillChar : emptyChar);
	SetColor();
	int digits = std::to_string(maxValue).length();
	cout << " " <<
		std::setw(digits) << value << "/" <<
		std::setw(digits) << maxValue;
}

void DrawLine(char ch, int width)
{
	cout << std::setfill(ch) << std::setw(width)
		<< "" << std::setfill(' ');
}

bool GetKey(int vKey)
{
	return GetAsyncKeyState(vKey) & 0x8000;
}
constexpr int KEY_COUNT = 256;
static bool prevDown[KEY_COUNT] = {};
static bool curDown[KEY_COUNT] = {};
bool GetKeyDown(int vKey)
{
	return curDown[vKey] && !prevDown[vKey];
	//bool cur = GetAsyncKeyState(vKey) & 0x8000;
	//bool down = cur && !prevDown[vKey];
	//prevDown[vKey] = cur;
	//return down;
}

void UpdateInput()
{
	for (int i = 0; i < 256; ++i)
	{
		prevDown[i] = curDown[i];
		curDown[i] = GetAsyncKeyState(i) & 0x8000;
	}
}

POINT GetMouseCellPos()
{
	HWND hwnd = GetConsoleWindow();
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	GetCurrentConsoleFontEx(handle, false, &fontInfo);

	POINT cellPos = { pt.x / fontInfo.dwFontSize.X,
	pt.y / fontInfo.dwFontSize.Y };
	return cellPos;
}

void FrameSync(int fps)
{
	static ULONGLONG prevTick = GetTickCount64();
	ULONGLONG curTick = GetTickCount64();
	ULONGLONG elapsed = curTick - prevTick;
	ULONGLONG targetTick = 1000 / fps;

	if (elapsed < targetTick)
		Sleep(static_cast<DWORD>(targetTick - elapsed));
	prevTick = GetTickCount64();
}

int GetCursorX()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return csbi.dwCursorPosition.X;
}

void SetConsoleFont(const wstring& fontName, COORD size, UINT weight)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX info = {};
	info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	info.dwFontSize = size; // ��, ����
	info.FontWeight = weight; // FW_ ~ 
	// ��Ʈ �̸� ����
	wcscpy_s(info.FaceName, fontName.c_str());
	SetCurrentConsoleFontEx(handle, false, &info);
}

