#include "stdafx.h"
#include "BmpImage.h"

using namespace std;

BmpImage::BmpImage(std::ostream *to) : to(to) {}

BmpImage::~BmpImage() {
	fill();

	delete to;
}

void BmpImage::initialize(nat w, nat h) {
	to->write("BM", 2);

	BmpHeader bh;
	bh.size = sizeof(BmpHeader) + sizeof(BmpInfoHeader) + 2;
	nat scanlineW = w * 3;
	if (scanlineW % 4 != 0) scanlineW += 4 - (scanlineW % 4);
	bh.size += scanlineW * h;
	bh.reserved = 0;
	bh.offset = sizeof(BmpHeader) + sizeof(BmpInfoHeader) + 2;
	to->write((char *)&bh, sizeof(BmpHeader));

	BmpInfoHeader bih;
	bih.size = sizeof(BmpInfoHeader);
	bih.width = w;
	bih.height = h;
	bih.planes = 1;
	bih.bpp = 24;
	bih.compression = 0;
	bih.sizeOfBmp = 0;
	bih.horzResolution = 1000;
	bih.vertResolution = 1000;
	bih.colorsUsed = 1 << bih.bpp;
	bih.colorsImportant = 0;

	to->write((char *)&bih, sizeof(BmpInfoHeader));

	atX = atY = 0;
	this->w = w;
	this->h = h;
}

void BmpImage::addPixel(byte r, byte g, byte b) {
	if (atX == w && atY == h) return;

	to->write((char *)&b, 1);
	to->write((char *)&g, 1);
	to->write((char *)&r, 1);

	atX++;
	if (atX == w) {
		nat totalSz = 3 * w;
		nat overlap = totalSz % 4;
		if (overlap != 0) {
			for (nat i = 0; i < 4 - overlap; i++) to->write("\0", 1);
		}
		atX = 0;
		atY++;
	}
}

void BmpImage::flush() {}

void BmpImage::fill() {
	nat count = 0;
	while (atX < w && atY < h) {
		addPixel(0, 0, 0);
		count++;
	}
	if (count) cout << "Filled " << count << " nonexisting pixels." << endl;
}