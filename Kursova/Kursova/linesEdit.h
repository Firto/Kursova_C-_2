#ifndef LINES_EDIT_H
#define LINES_EDIT_H

#include <windows.h>
#include <iostream>
#include <conio.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <iomanip>

void cl();

void gotoxy(int x, int y);

int GetBufferChars();

int GetBufferCharsbot();

const POINT GetPosCur();

void clearToPos(POINT pos);

#endif
