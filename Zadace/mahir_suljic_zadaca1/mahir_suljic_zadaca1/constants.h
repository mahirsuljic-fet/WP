#pragma once

#include "framework.h"

namespace Colors {
	constexpr COLORREF WHITE = RGB(255, 255, 255);
	constexpr COLORREF BLACK = RGB(0, 0, 0);

	constexpr COLORREF RED = RGB(255, 0, 0);
	constexpr COLORREF GREEN = RGB(0, 255, 0);
	constexpr COLORREF BLUE = RGB(0, 0, 255);

	constexpr COLORREF GRAY  = RGB(128, 128, 128);
	constexpr COLORREF YELLOW = RGB(255, 255, 0);
	constexpr COLORREF DARK_YELLOW = RGB(200, 200, 0);

	constexpr COLORREF BG_VLINE[] = {
		RED,
		GREEN,
		BLUE
	};

	constexpr COLORREF BG_HLINE[] = {
		RGB(200, 0, 200),
		RGB(50, 0, 50)
	};

	constexpr COLORREF SQUARE[] = {
		GREEN,
		BLUE,
		YELLOW,
		RED
	};

	constexpr COLORREF BELGIUM[] = {
		BLACK,
		DARK_YELLOW,
		RED
	};

	constexpr COLORREF GERMANY[] = {
		BELGIUM[0],
		BELGIUM[2],
		BELGIUM[1]
	};

	constexpr COLORREF FLAG_TEXT = WHITE;

	constexpr COLORREF STAR = BLACK;
	constexpr COLORREF INITIALS = BLACK;

	constexpr COLORREF CIRCLE_OUTLINE = BLACK;
	constexpr COLORREF CIRCLE_HATCH = WHITE;
	constexpr COLORREF CIRCLE_SOLID = BLACK;

	constexpr COLORREF DIAMOND_OUTLINE = BLACK;
	constexpr COLORREF SQUARE_OUTLINE = BLACK; // WARNING: not used because of PS_NULL style
	constexpr COLORREF SQUARE_INSIDE = GRAY;
}

namespace Sizes {
	constexpr int BG_LINE_SIZE = 10;
	constexpr int INITIALS_LINE_SIZE = 15;

	constexpr int CIRCLE_OUTLINE_WIDTH = 5;
	constexpr int DIAMOND_OUTLINE_WIDTH = 3;
	constexpr int SQUARE_OUTLINE_WIDTH = 1;

	constexpr int CIRCLE_RADIUS = 300;
}
