#include "stdafx.h"

#include "ImageReader.h"
#include <iostream>
#include <iomanip>

using namespace std;

ImageReader::ImageReader(Configuration &config, Image *to) : to(to), config(config) {
	scanline = new Color[config.xPixels()];
	flushedLines = 0;
	to->initialize(config.xPixels(), config.yPixels());
}

ImageReader::~ImageReader() {
	fillImage();
	to->finish();
	delete to;
}

bool ImageReader::addData(Message &data) {
	nat type = data.getNat(0 * 4);
	nat line = data.getNat(1 * 4);
	nat height = data.getNat(2 * 4);
	nat mode = data.getNat(3 * 4);
	nat width = data.getNat(4 * 4);
	nat width2 = data.getNat(5 * 4);

	cout << dec << setw(5) << "Packet: " << type << " " << line << " " << height << " " << mode << " " << width << " " << width2 << " " << data.getSize() - headerSize << endl;
	
	//BOOST_ASSERT(type == 10); //The only one tested so far.

	if (type == 10) {

		nat myWidth = config.xPixels();
		for (nat y = 0; y < height; y++) {
			const byte *d = data.getData() + headerSize + (y * width);

			nat totalY = line + y;
			nat channel = totalY % 3;

			addScanline(d, myWidth, channel);
			if (channel == 2) flushScanline();
		}

		return true;
	} else if (type == 12) {
		nat myWidth = config.xPixels();
		for (nat y = 0; y < height; y++) {
			const byte *d = data.getData() + headerSize + (y * width);

			nat totalY = line + y;
			nat channel = totalY % 3;

			addReverseScanline(d + (width - myWidth), myWidth, channel);
			if (channel == 2) flushScanline();
		}

		return true;
	}

	return false;
}

void ImageReader::addReverseScanline(const byte *data, nat width, nat channel) {
	nat x = width;
	do {
		x--;
		scanline[x][channel] = *data;
		data++;
	} while (x > 0);
}

void ImageReader::addScanline(const byte *data, nat width, nat channel) {
	for (nat x = 0; x < width; x++, data++) {
		scanline[x][channel] = *data;
	}
}

void ImageReader::flushScanline() {
	to->addLine(scanline);
	flushedLines++;
}

void ImageReader::fillImage() {
	nat h = config.yPixels();
	nat w = config.xPixels();

	if (flushedLines < h) for (nat x = 0; x < w; x++) scanline[x] = Color(255, 255, 255);
	while (flushedLines < h) {
		flushScanline();
	}
}

void ImageReader::dump() {
	cout << dec;
	cout << "Image: " << config.xPixels() << " x " << config.yPixels() << endl;
	cout << "DPI: " << config.dpi << endl;
	cout << "Color type: " << config.colorType << endl;
}
