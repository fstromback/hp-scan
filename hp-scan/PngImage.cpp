#include "stdafx.h"
#include "PngImage.h"

#include <iostream>

using namespace std;

PngImage::PngImage(ostream *to) : to(to) {
	pngPtr = null;
	pngInfo = null;
	tmpLine = null;
	initPng();
}

PngImage::~PngImage() {
	if (pngInfo) png_free_data(pngPtr, pngInfo, PNG_FREE_ALL, -1);
	if (pngPtr) png_destroy_write_struct(&pngPtr, (png_infopp)null);

	if (tmpLine) delete tmpLine;
	if (to) delete to;
}

void PngImage::initialize(nat w, nat h, nat dpi) {
	if (pngInfo == null) return;

	//png_set_read_fn(pngPtr, this, &PngImage::read);
	png_set_write_fn(pngPtr, this, &PngImage::write, &PngImage::flushPng);

	png_set_IHDR(pngPtr, pngInfo, w, h, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_text title_text;
	title_text.compression = PNG_TEXT_COMPRESSION_NONE;
	title_text.key = "Title";
	title_text.text = "Scanned image";
	png_set_text(pngPtr, pngInfo, &title_text, 1);

	png_write_info(pngPtr, pngInfo);

	tmpLine = new byte[w * 3];
	rowAtY = 0;
	width = w;
	height = h;
}

void PngImage::addLine(Color *scanline) {
	if (rowAtY >= height) return;
	rowAtY++;

	byte *at = tmpLine;
	for (nat i = 0; i < width; i++) {
		*(at++) = scanline[i].r;
		*(at++) = scanline[i].g;
		*(at++) = scanline[i].b;
	}

	png_write_row(pngPtr, tmpLine);
}


void PngImage::finish() {
	BOOST_ASSERT(rowAtY == height);
	png_write_end(pngPtr, pngInfo);
}

void PngImage::initPng() {
	pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, null, null, null);
	if (pngPtr == null) {
		cout << "Failed to initialize LIBPNG!" << endl;
		return;
	}

	pngInfo = png_create_info_struct(pngPtr);
	if (pngInfo == null) {
		cout << "Failed to initialize LIBPNG!" << endl;
		return;
	}
}

void PngImage::read(png_structp pngPtr, png_bytep to, png_size_t sz) {
	PngImage *img = (PngImage *)png_get_io_ptr(pngPtr);
	//Cannot read...
}

void PngImage::write(png_structp pngPtr, png_bytep to, png_size_t sz) {
	PngImage *img = (PngImage *)png_get_io_ptr(pngPtr);
	img->to->write((char *)to, sz);
}

void PngImage::flushPng(png_structp pngPtr) {
	//Nothing to do here...
}