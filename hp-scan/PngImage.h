#pragma once

#include "Image.h"

#include <png.h>

#include <string>

using namespace std;

class PngImage : public Image {
public:
	PngImage(ostream *to);
	virtual ~PngImage();

	virtual void initialize(nat w, nat h, nat dpi);
	virtual void addLine(Color *scanline);
	virtual void finish();
	static string extension() { return "png"; };
private:
	ostream *to;
	png_structp pngPtr;
	png_infop pngInfo;

	nat rowAtY;
	nat width;
	nat height;
	byte *tmpLine;

	void initPng();

	static void PNGAPI read(png_structp pngPtr, png_bytep to, png_size_t sz);
	static void PNGAPI write(png_structp pngPtr, png_bytep to, png_size_t sz);
	static void PNGAPI flushPng(png_structp pngPtr);
};
