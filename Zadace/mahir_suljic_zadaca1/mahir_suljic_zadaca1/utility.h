#pragma once

#include "framework.h"

#define EL_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

POINT getRectCenter(const RECT& rect);

void translatePoints(POINT* points, int point_count, POINT pos);
