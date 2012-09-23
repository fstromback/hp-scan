#pragma once

#include "Image.h"
#include <iostream>

//NOTE: Currently does not work on other platforms than x86
class BmpImage : public Image {
public:
	BmpImage(std::ostream *to);
	virtual ~BmpImage();

	virtual void initialize(nat w, nat h);
	virtual void addPixel(byte r, byte g, byte b);
	virtual void flush();
private:
	std::ostream *to;

	struct BmpHeader {
		nat size;
		nat reserved;
		nat offset;
	};

	struct BmpInfoHeader {
		nat size;
		nat width;
		nat height;
		short planes;
		short bpp;
		nat compression;
		nat sizeOfBmp;
		nat horzResolution;
		nat vertResolution;
		nat colorsUsed;
		nat colorsImportant;
	};

	nat atX;
	nat atY;
	nat w;
	nat h;

	void fill();
};
