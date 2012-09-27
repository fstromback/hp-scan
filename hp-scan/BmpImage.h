#pragma once

#include "Image.h"
#include <iostream>

//NOTE: Currently does not work on other platforms than x86
class BmpImage : public Image {
public:
	BmpImage(std::ostream *to);
	virtual ~BmpImage();

	virtual void initialize(nat w, nat h, nat dpi);
	virtual void addLine(Color *line);
	virtual void finish();
	static string extension() { return "bmp"; };
private:
	std::ostream *to;

	enum { bmpHeaderSize = 14, bmpInfoHeaderSize = 40 };

	nat atY;
	nat w;
	nat h;
	nat scanlineWidth;

	void writeBmpHeader();
	void writeBmpInfoHeader();

	void write(nat i);
	void write(short i);
};
