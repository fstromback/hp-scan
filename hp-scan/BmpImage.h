#pragma once

#include "Image.h"
#include <iostream>

//NOTE: Currently does not work on other platforms than x86
class BmpImage : public Image {
public:
	BmpImage(std::ostream *to);
	virtual ~BmpImage();

	virtual void initialize(nat w, nat h);
	virtual void addLine(Color *line);
	virtual void finish();
private:
	std::ostream *to;

	enum { bmpHeaderSize = 14, bmpInfoHeaderSize = 40 };

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

	nat atY;
	nat w;
	nat h;
	nat scanlineWidth;

	void writeBmpHeader();
	void writeBmpInfoHeader();

	void write(nat i);
	void write(short i);
};
