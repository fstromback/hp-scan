#pragma once

#include "Color.h"

class Image {
public:
	Image();
	virtual ~Image();

	virtual void initialize(nat w, nat h) = 0;
	virtual void addLine(Color *scanline) = 0;
	virtual void finish() = 0;
};
