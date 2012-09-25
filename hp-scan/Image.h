#pragma once

#include "Color.h"

#include <string>

using namespace std;

class Image {
public:
	Image();
	virtual ~Image();

	virtual void initialize(nat w, nat h) = 0;
	virtual void addLine(Color *scanline) = 0;
	virtual void finish() = 0;
	static string extension() { return ""; }; //Derived classes should "override" this.
};
