#include "draw.h"
#include "constants.h"
#include "utility.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <functional>
#include <stdexcept>

void drawHLines(HDC hdc, RECT drawArea, const COLORREF* colors, int color_count, int line_size, int spacing)
{
	const int iterations = ((drawArea.bottom - drawArea.top) / line_size) / (spacing + 1);

	HBRUSH* brushes = new HBRUSH[color_count];

	for (int i = 0; i < color_count; ++i)
		brushes[i] = CreateSolidBrush(colors[i]);

	RECT line = { drawArea.left, drawArea.top, drawArea.right, drawArea.top + line_size };

	for (int i = 0; i <= iterations; ++i)
	{
		int brush_index = color_count > 1 ? i % color_count : 0;
		HBRUSH brush = brushes[brush_index];
		FillRect(hdc, &line, brush);

		line.top += line_size * (spacing + 1);
		line.bottom += line_size * (spacing + 1);

		if (line.bottom > drawArea.bottom)
			line.bottom = drawArea.bottom;
	}

	for (int i = 0; i < color_count; ++i)
		DeleteObject(brushes[i]);

	delete[] brushes;
}

void drawVLines(HDC hdc, RECT drawArea, const COLORREF* colors, int color_count, int line_size, int spacing)
{
	const int iterations = ((drawArea.right - drawArea.left) / line_size) / (spacing + 1);

	HBRUSH* brushes = new HBRUSH[color_count];

	for (int i = 0; i < color_count; ++i)
		brushes[i] = CreateSolidBrush(colors[i]);

	RECT line = { drawArea.left, drawArea.top, drawArea.left + line_size, drawArea.bottom };

	for (int i = 0; i <= iterations; ++i)
	{
		int brush_index = color_count > 1 ? i % color_count : 0;
		HBRUSH brush = brushes[brush_index];
		FillRect(hdc, &line, brush);

		line.left += line_size * (spacing + 1);
		line.right += line_size * (spacing + 1);

		if (line.right > drawArea.right)
			line.right = drawArea.right;
	}

	for (int i = 0; i < color_count; ++i)
		DeleteObject(brushes[i]);

	delete[] brushes;
}

void drawLines(HDC hdc, RECT drawArea)
{
	drawVLines(hdc, drawArea, Colors::BG_VLINE, EL_COUNT(Colors::BG_VLINE), Sizes::BG_LINE_SIZE, 0);
	drawHLines(hdc, drawArea, Colors::BG_HLINE, EL_COUNT(Colors::BG_HLINE), Sizes::BG_LINE_SIZE, 1);
}

void drawStar(HDC hdc, POINT pos, size_t radius, HBRUSH brush)
{
	constexpr int point_count = 10;
	constexpr float step = 2 * M_PI / point_count;

	const int r_out = radius;
	const int r_in = radius / 2.55;

	POINT points[point_count];

	float fi = M_PI_2;
	for (int i = 0; i < point_count; ++i)
	{
		int len = i % 2 ? r_out : r_in;

		int x = pos.x + cos(fi) * len;
		int y = pos.y + sin(fi) * len;

		points[i] = { x, y };

		fi += step;
	}

	Polygon(hdc, points, point_count);
}

void drawStars(HDC hdc, RECT drawArea, double scale)
{
	const int radius = 50 * scale;
	const int offset = radius * 1.25;

	HBRUSH star_brush = CreateSolidBrush(Colors::STAR);

	POINT positions[] = {
		{ drawArea.left + offset, drawArea.top + offset },
		{ drawArea.left + offset, drawArea.bottom - offset },
		{ drawArea.right - offset, drawArea.top + offset },
		{ drawArea.right - offset, drawArea.bottom - offset }
	};

	SelectObject(hdc, star_brush);

	for (int i = 0; i < EL_COUNT(positions); ++i)
		drawStar(hdc, positions[i], radius, star_brush);

	DeleteObject(star_brush);
}

void drawCircleBackground(HDC hdc, POINT pos, int radius)
{
	const HPEN pen = CreatePen(PS_SOLID, Sizes::CIRCLE_OUTLINE_WIDTH, Colors::CIRCLE_OUTLINE);
	const HBRUSH hbrush = CreateHatchBrush(HS_DIAGCROSS, Colors::CIRCLE_HATCH);

	const RECT circle_bounds = {
		pos.x - radius / 2,
		pos.y - radius / 2,
		pos.x + radius / 2,
		pos.y + radius / 2
	};

	SetBkColor(hdc, Colors::CIRCLE_SOLID);

	SelectObject(hdc, pen);
	SelectObject(hdc, hbrush);

	Ellipse(hdc, circle_bounds.left, circle_bounds.top, circle_bounds.right, circle_bounds.bottom);

	DeleteObject(hbrush);
	DeleteObject(pen);
}

void drawCircleDiamond(HDC hdc, POINT pos, int length, int color_state)
{
	constexpr int pps = 4; // points per shape

	const int l2 = length / 2;
	const int l4 = length / 4;

	const HPEN null_pen = CreatePen(PS_NULL, Sizes::DIAMOND_OUTLINE_WIDTH, Colors::DIAMOND_OUTLINE);
	const HPEN outline_pen = CreatePen(PS_SOLID, Sizes::DIAMOND_OUTLINE_WIDTH, Colors::DIAMOND_OUTLINE);

	const HBRUSH brushes[] = {
		CreateSolidBrush(Colors::SQUARE[(color_state + 0) % 4]),
		CreateSolidBrush(Colors::SQUARE[(color_state + 1) % 4]),
		CreateSolidBrush(Colors::SQUARE[(color_state + 2) % 4]),
		CreateSolidBrush(Colors::SQUARE[(color_state + 3) % 4])
	};

	POINT points[] = {
		{ 0, 0 },
		{ l4, l4 },
		{ l2, 0 },
		{ l4, -l4 },

		{ 0, 0 },
		{ -l4, l4 },
		{ 0, l2 },
		{ l4, l4 },

		{ 0, 0 },
		{ -l4, l4 },
		{ -l2, 0 },
		{ -l4, -l4 },

		{ 0, 0 },
		{ l4, -l4 },
		{ 0, -l2 },
		{ -l4, -l4 },

		// outline
		{ -l2, 0 },
		{ 0, -l2 },
		{ l2, 0 },
		{ 0, l2 },
	};

	translatePoints(points, EL_COUNT(points), pos);

	SelectObject(hdc, null_pen);
	for (int i = 0; i < EL_COUNT(brushes); ++i)
	{
		SelectObject(hdc, brushes[i]);
		Polygon(hdc, points + i * pps, pps);
	}

	SelectObject(hdc, GetStockObject(NULL_BRUSH));
	SelectObject(hdc, outline_pen);
	Polygon(hdc, points + EL_COUNT(brushes) * pps, pps);

	for (int i = 0; i < EL_COUNT(brushes); ++i)
		DeleteObject(brushes[i]);

	DeleteObject(outline_pen);
	DeleteObject(null_pen);
}

void drawCircleSquare(HDC hdc, POINT pos, int length, int color_state)
{
	constexpr int pps = 3; // points per shape

	const int l2 = length / 2;

	const HPEN pen = CreatePen(PS_NULL, Sizes::SQUARE_OUTLINE_WIDTH, Colors::SQUARE_OUTLINE);

	const HBRUSH hbrush = CreateHatchBrush(HS_HORIZONTAL, Colors::SQUARE_INSIDE);
	const HBRUSH vbrush = CreateHatchBrush(HS_VERTICAL, Colors::SQUARE_INSIDE);

	POINT points[] = {
		{ 0, 0 },
		{ l2, l2 },
		{ l2, -l2 },

		{ 0, 0 },
		{ -l2, l2 },
		{ l2, l2 },

		{ 0, 0 },
		{ -l2, l2 },
		{ -l2, -l2 },

		{ 0, 0 },
		{ l2, -l2 },
		{ -l2, -l2 },
	};

	SelectObject(hdc, pen);
	translatePoints(points, EL_COUNT(points), pos);

	for (int i = 0; i < EL_COUNT(Colors::SQUARE); ++i)
	{
		const HBRUSH brush = i % 2 ? hbrush : vbrush;
		const COLORREF color = Colors::SQUARE[(color_state + i) % 4];

		SetBkColor(hdc, color);
		SelectObject(hdc, brush);
		Polygon(hdc, points + i * pps, pps);
	}

	DeleteObject(vbrush);
	DeleteObject(hbrush);
	DeleteObject(pen);
}

void drawCircle(HDC hdc, RECT drawArea, double scale)
{
	static int color_state = 0;

	const POINT center = getRectCenter(drawArea);

	drawCircleBackground(hdc, center, scale * Sizes::CIRCLE_RADIUS);
	drawCircleDiamond(hdc, center, scale * Sizes::CIRCLE_RADIUS * 0.75, color_state);
	drawCircleSquare(hdc, center, scale * Sizes::CIRCLE_RADIUS * 0.25, 4 - color_state);

	++color_state;
	color_state %= 4;
}

void drawFirstInitial(HDC hdc, POINT pos, SIZE size)
{
	SIZE hs = { size.cx / 2, size.cy / 2 }; // half-size

	POINT points[] = {
		{hs.cx,  hs.cy},
		{hs.cx, -hs.cy},
		{0, 0},
		{-hs.cx, -hs.cy},
		{-hs.cx,  hs.cy}
	};

	translatePoints(points, EL_COUNT(points), pos);

	POINT prev_point;
	MoveToEx(hdc, points[0].x, points[0].y, &prev_point);

	for (int i = 0; i < EL_COUNT(points); ++i)
		LineTo(hdc, points[i].x, points[i].y);

	MoveToEx(hdc, prev_point.x, prev_point.y, NULL);
}

void drawSecondInitial(HDC hdc, POINT pos, SIZE size)
{
	SIZE hs = { size.cx / 2, size.cy / 2 }; // half-size

	POINT points[] = {
		// prva cetvrtina kruga
		{0, 0},
		{hs.cx, 0},
		{hs.cx,  hs.cy / 2},
		{hs.cx,  hs.cy / 2},

		// gornja polovina kruga
		{hs.cx,  hs.cy / 2},
		{hs.cx,  hs.cy},
		{-hs.cx, hs.cy},
		{-hs.cx, hs.cy / 2}
	};

	const int point_count = EL_COUNT(points);
	const int ppc = 4; // points per curve

	POINT reflected[point_count];

	for (int i = 0; i < point_count; ++i)
		reflected[i] = { -points[i].x, -points[i].y };

	translatePoints(points, point_count, pos);
	translatePoints(reflected, point_count, pos);

	PolyBezier(hdc, points, ppc);
	PolyBezier(hdc, points + ppc, ppc);
	PolyBezier(hdc, reflected, ppc);
	PolyBezier(hdc, reflected + ppc, ppc);
}

void drawInitials(HDC hdc, RECT drawArea, double scale)
{
	const int offset = 300 * scale;

	const POINT center = getRectCenter(drawArea);
	const HPEN pen = CreatePen(PS_SOLID, Sizes::INITIALS_LINE_SIZE * scale, Colors::INITIALS);

	const SIZE firstInitialSize = { 150 * scale, 250 * scale };
	const SIZE secondInitialSize = firstInitialSize;

	SelectObject(hdc, pen);

	drawFirstInitial(hdc, { center.x - offset, center.y }, firstInitialSize);
	drawSecondInitial(hdc, { center.x + offset, center.y }, secondInitialSize);

	DeleteObject(pen);
}

void drawFlag(HDC hdc, RECT drawArea, FLAG_TYPE flag_type)
{
	int stripe_count;
	int line_size;

	std::string text;
	std::function<void(HDC, RECT, const COLORREF*, int, int, int)> drawFunc;

	POINT center;
	SIZE text_size;
	POINT text_pos;
	const COLORREF* colors;

	switch (flag_type) {

	case GERMANY:
		text = "GERMANY";
		colors = Colors::GERMANY;
		stripe_count = EL_COUNT(Colors::GERMANY);
		line_size = (drawArea.bottom - drawArea.top) / stripe_count;
		drawFunc = drawHLines;
		break;

	case BELGIUM:
		text = "BELGIUM";
		colors = Colors::BELGIUM;
		stripe_count = EL_COUNT(Colors::BELGIUM);
		line_size = (drawArea.right - drawArea.left) / stripe_count;
		drawFunc = drawVLines;
		break;

	default:
		throw std::invalid_argument("Unknown flag type");
	};

	GetTextExtentPoint32A(hdc, text.c_str(), text.size(), &text_size);

	center = getRectCenter(drawArea);

	text_pos = {
		center.x - text_size.cx / 2,
		center.y - text_size.cy / 2
	};

	drawFunc(hdc, drawArea, colors, stripe_count, line_size, 0);

	int prev_mode = SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, Colors::FLAG_TEXT);
	TextOutA(hdc, text_pos.x, text_pos.y, text.c_str(), text.size());
	SetBkMode(hdc, prev_mode);
}

void drawBelgium(HDC hdc, RECT drawArea)
{
	drawFlag(hdc, drawArea, BELGIUM);
}

void drawGermany(HDC hdc, RECT drawArea)
{
	drawFlag(hdc, drawArea, GERMANY);
}
