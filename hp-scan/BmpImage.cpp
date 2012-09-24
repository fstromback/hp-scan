#include "stdafx.h"
#include "BmpImage.h"

using namespace std;

BmpImage::BmpImage(std::ostream *to) : to(to) {}

BmpImage::~BmpImage() {
	delete to;
}

void BmpImage::initialize(nat w, nat h) {
	this->w = w;
	this->h = h;
	atY = 0;
	scanlineWidth = w * 3;
	if (scanlineWidth % 4 != 0) scanlineWidth += 4 - (scanlineWidth % 4);

	writeBmpHeader();

	writeBmpInfoHeader();
}

void BmpImage::writeBmpHeader() {
	to->write("BM", 2);

	nat fileSize = bmpHeaderSize + bmpInfoHeaderSize; //Size of all headers
	fileSize += scanlineWidth * h;
	write(fileSize);
	write(nat(0));
	write(nat(54));
}

void BmpImage::writeBmpInfoHeader() {
	write(nat(bmpInfoHeaderSize)); //Size of the BmpInfoHeader
	write(w);
	write(h);
	write(short(1)); //Planes
	write(short(24)); //BPP
	write(nat(0)); //Compression
	write(nat(0)); //Size of bmp data
	write(nat(1000)); //Horizontal resolution
	write(nat(1000)); //Vertical resolution
	write(nat(1 << 24)); //colors used
	write(nat(0)); //Important colors
}

void BmpImage::addLine(Color *line) {
	if (atY >= h) return;

	nat pos = bmpHeaderSize + bmpInfoHeaderSize;
	int z = (h - atY - 1);
	pos += scanlineWidth * (h - atY - 1);
	to->seekp(pos);

	for (nat x = 0; x < w; x++) {
		to->write((char *)&line[x].b, 1);
		to->write((char *)&line[x].g, 1);
		to->write((char *)&line[x].r, 1);
	}
	for (nat x = 0; x < (scanlineWidth - (w * 3)); x++) {
		to->put('\0');
	}
	atY++;
}

void BmpImage::finish() {}

void BmpImage::write(short i) {
	char t = i & 0xFF;
	to->write(&t, 1);
	t = (i & 0xFF00) >> 8;
	to->write(&t, 1);
}

void BmpImage::write(nat i) {
	char t = i & 0xFF;
	to->write(&t, 1);
	t = (i & 0xFF00) >> 8;
	to->write(&t, 1);
	t = (i & 0xFF0000) >> 16;
	to->write(&t, 1);
	t = (i & 0xFF000000) >> 24;
	to->write(&t, 1);
}
