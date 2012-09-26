#pragma once

#include "Point.h"

#include <string>

using namespace std;

class Rect {
public:
	Rect();
	Rect(nat w, nat h);
	Rect(nat top, nat left, nat right, nat bottom);
	Rect(const Point &topLeft, const Point &bottomRight);
	
	nat top, left, right, bottom;

	inline nat width() const { return right - left; };
	inline nat height() const { return bottom - top; };

	static Rect paperSize(string descriptor); //Returns standard paper sizes in 1/100 of an inch
};
