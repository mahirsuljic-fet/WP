#pragma once

#include "framework.h"

enum FLAG_TYPE {
	GERMANY,
	BELGIUM
};

void drawHLines(HDC hdc, RECT drawArea, const COLORREF* colors, int color_count, int line_size, int spacing = 0);
void drawVLines(HDC hdc, RECT drawArea, const COLORREF* colors, int color_count, int line_size, int spacing = 0);
void drawLines(HDC hdc, RECT drawArea);

void drawStar(HDC hdc, POINT pos, size_t size, HBRUSH brush);
void drawStars(HDC hdc, RECT drawArea, double scale);

void drawCircleBackground(HDC hdc, POINT pos, int radius);
void drawCircleDiamond(HDC hdc, POINT pos, int length, int color_state);
void drawCircleSquare(HDC hdc, POINT pos, int length, int color_state);
void drawCircle(HDC hdc, RECT drawArea, double scale);

void drawFirstInitial(HDC hdc, POINT pos, SIZE size);
void drawSecondInitial(HDC hdc, POINT pos, SIZE size);
void drawInitials(HDC hdc, RECT drawArea, double scale);

void drawBelgium(HDC hdc, RECT drawArea);
void drawGermany(HDC hdc, RECT drawArea);
void drawFlag(HDC hdc, RECT drawArea, FLAG_TYPE flag_type);
