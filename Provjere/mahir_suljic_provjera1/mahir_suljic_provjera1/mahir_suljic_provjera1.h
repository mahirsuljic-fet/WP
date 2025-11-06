#pragma once

#include "resource.h"

void drawFrame(HDC hdc, RECT windowArea, RECT mainArea);
void drawFirstFlag(HDC hdc, RECT mainArea);
void drawSecondFlag(HDC hdc, RECT mainArea, double scale);
