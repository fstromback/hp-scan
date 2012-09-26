#include "stdafx.h"
#include "Rect.h"

#include <boost/algorithm/string.hpp>

Rect::Rect() : left(0), top(0), right(0), bottom(0) {}

Rect::Rect(nat w, nat h) : left(0), top(0), right(w), bottom(h) {}

Rect::Rect(nat left, nat top, nat right, nat bottom) : left(left), top(top), right(right), bottom(bottom) {}

Rect::Rect(const Point &topLeft, const Point &bottomRight) : left(topLeft.x), top(topLeft.y), right(bottomRight.x), bottom(bottomRight.y) {}


Rect Rect::paperSize(string descriptor) {
	boost::to_upper(descriptor);
	if (descriptor == "A4") return Rect(827, 1100);
	
	return Rect();
}