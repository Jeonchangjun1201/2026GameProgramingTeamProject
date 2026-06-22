#include "EndingArt.h"
#include "Console.h"

void RenderGameOverArt()
{
	SetColor(Color::LIGHT_RED);
	GotoXY(18, 2);  cout << "     _.-^^---....,,--       ";
	GotoXY(18, 3);  cout << " _--                  --_  ";
	GotoXY(18, 4);  cout << "<                        >)";
	GotoXY(18, 5);  cout << "|                         | ";
	GotoXY(18, 6);  cout << " \\._                   _./  ";
	GotoXY(18, 7);  cout << "    ```--. . , ; .--'''       ";
	GotoXY(18, 8);  cout << "          | |   |             ";
	GotoXY(18, 9);  cout << "       .-=||  | |=-.   ";
	GotoXY(18, 10); cout << "       `-=#$%&%$#=-'   ";
	GotoXY(18, 11); cout << "          | ;  :|     ";
	GotoXY(18, 12); cout << " _____.,-#%&$@%#&#~,._____";

	GotoXY(22, 14); cout << " _____     _ _ ";
	GotoXY(22, 15); cout << " |  ___|_ _(_) |";
	GotoXY(22, 16); cout << " | |_ / _` | | |";
	GotoXY(22, 17); cout << " |  _| (_| | | |";
	GotoXY(22, 18); cout << " |_|  \\__,_|_|_|";
	SetColor();
	GotoXY(24, 21); cout << "Press ENTER to return";
}

void RenderWinArt()
{
	SetColor(Color::LIGHT_GREEN);
	GotoXY(18, 2);  cout << "  ___ _   _  ___ ___ ___  ___ ___ ";
	GotoXY(18, 3);  cout << " / __| | | |/ __/ __/ _ \\/ __/ __|";
	GotoXY(18, 4);  cout << " \\__ \\ |_| | (_| (_|  __/\\__ \\__ \\";
	GotoXY(18, 5);  cout << " |___/\\__,_|\\___\\___\\___||___/___/";
	SetColor();
	SetUnicodeMode();
	GotoXY(2, 7);  wcout << L"⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣴⣶⣾⣿⣿⣿⣿⣷⣶⣦⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀";
	GotoXY(2, 8);  wcout << L"⠀⠀⠀⠀⠀⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣿⣿⣦⢄⠀⠀⠀⠀⠀";
	GotoXY(2, 9);  wcout << L"⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀";
	GotoXY(2, 10); wcout << L"⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⠀⠀";
	GotoXY(2, 11); wcout << L"⣼⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠉⠉⠙⢿⣿⣿⣿⣿⣧⠀";
	GotoXY(2, 12); wcout << L"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⣠⣾⡷⠀⢈⣿⣿⣿⣿⣿⡆";
	GotoXY(2, 13); wcout << L"⣿⣿⣿⣿⣿⣿⡿⠋⠁⠈⠙⢿⣿⣿⡿⠋⠀⣠⣾⡿⠋⠀⣠⣾⣿⣿⣿⣿⣿⣷";
	GotoXY(2, 14); wcout << L"⣿⣿⣿⣿⣿⣿⡁⠀⢾⣷⣄⠀⠙⠉⠀⣠⣾⡿⠋⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿";
	GotoXY(2, 15); wcout << L"⣿⣿⣿⣿⣿⣿⣷⣄⠀⠙⢿⣷⣤⣤⣾⡿⠋⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿";
	GotoXY(2, 16); wcout << L"⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⠙⢿⡿⠋⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇";
	GotoXY(2, 17); wcout << L"⠫⢻⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⣀⣀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀";
	GotoXY(2, 18); wcout << L"⠀⠀⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀";
	GotoXY(2, 19); wcout << L"⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀";
	GotoXY(2, 20); wcout << L"⠀⠀⠀⠀⠀⠉⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠉⠀⠀⠀⠀⠀";
	GotoXY(2, 21); wcout << L"⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⢿⣿⣿⣿⣿⡿⠿⠟⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀";
	SetDefaultMode();
	GotoXY(24, 24); cout << "Press ENTER to return";
}