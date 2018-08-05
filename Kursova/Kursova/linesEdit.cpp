#include <windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
using namespace std;

void cl() {
	system("cls");
}

void gotoxy(int x, int y) {
	COORD coordinates = { x, y };
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(outputHandle, coordinates);
}

int GetBufferChars(){
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	GetConsoleScreenBufferInfo(hCon, &csbInfo);
	return csbInfo.srWindow.Right - csbInfo.srWindow.Left;

}

int GetBufferCharsbot() {
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	GetConsoleScreenBufferInfo(hCon, &csbInfo);
	return csbInfo.srWindow.Bottom - csbInfo.srWindow.Top;
}

const POINT GetPosCur() {
	POINT pos;
	CONSOLE_SCREEN_BUFFER_INFO bi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bi);
	pos.x = bi.dwCursorPosition.X;
	pos.y = bi.dwCursorPosition.Y;
	return pos;
}

void clearToPos(POINT pos) {
	POINT current_pos = GetPosCur();
	while (current_pos.y >= pos.y)
	{
		if (current_pos.y > pos.y) {
			gotoxy(0, current_pos.y);
			for (int i = 0; i < GetBufferChars(); i++)
			{
				cout << '\0';
			}
		}
		else if (current_pos.y == pos.y) {
			gotoxy(pos.x, current_pos.y);
			if (current_pos.x != pos.x) for (int i = 0; i < GetBufferChars() - pos.x; i++)
			{
				cout << '\0';
			}
		}

		current_pos.y--;
	}
	gotoxy(pos.x, pos.y);
}
