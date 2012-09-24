#pragma once

#include "Image.h"

#include "png.h"

#include <string>

using namespace std;

class PngImage : public Image {
public:
	PngImage(ostream *to);
	virtual ~PngImage();

	virtual void initialize(nat w, nat h);
	virtual void addPixel(byte r, byte g, byte b);
	virtual void flush();
private:
	ostream *to;
	png_structp pngPtr;
	png_infop pngInfo;
	nat rowAtX;
	nat rowAtY;
	nat width;
	nat height;
	byte *row;

	void initPng();

	static void PNGAPI read(png_structp pngPtr, png_bytep to, png_size_t sz);
	static void PNGAPI write(png_structp pngPtr, png_bytep to, png_size_t sz);
	static void PNGAPI flushPng(png_structp pngPtr);
};
