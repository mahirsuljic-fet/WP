#include "utility.h"

POINT getRectCenter(const RECT& rect)
{
	return {
		rect.left + (rect.right - rect.left) / 2,
		rect.top + (rect.bottom - rect.top) / 2
	};
}

void translatePoints(POINT* points, int point_count, POINT pos)
{
	for (int i = 0; i < point_count; ++i)
	{
		points[i].x += pos.x;
		points[i].y += pos.y;
	}
}
