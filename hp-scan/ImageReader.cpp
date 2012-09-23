#include "stdafx.h"

#include "ImageReader.h"
#include <iostream>
#include <iomanip>

using namespace std;

ImageReader::ImageReader(Configuration &config, Image *to) : buffer(config.xPixels(), config.yPixels(), 3), to(to), config(config) {
	to->initialize(config.xPixels(), config.yPixels());
}

ImageReader::~ImageReader() {
	buffer.flush(to);
	delete to;
}

void ImageReader::addData(Message &data) {
	nat type = data.getNat(0 * 4);
	nat line = data.getNat(1 * 4);
	nat height = data.getNat(2 * 4);
	nat mode = data.getNat(3 * 4);
	nat width = data.getNat(4 * 4);
	nat width2 = data.getNat(5 * 4);

	cout << dec << setw(5) << "Packet: " << type << " " << line << " " << height << " " << mode << " " << width << " " << width2 << " " << data.getSize() - headerSize << endl;
	
	BOOST_ASSERT(type == 10); //The only one tested so far.

	nat myWidth = config.xPixels();
	for (nat y = 0; y < height; y++) {
		const byte *d = data.getData() + headerSize + (y * width);
		nat totalY = line + y;
		nat channel = totalY % 3;
		totalY /= 3;
		for (nat x = 0; x < myWidth; x++) {
			buffer.at(x, totalY, channel) = *d;
			d++;
		}
	}
}


void ImageReader::dump() {
	cout << dec;
	cout << "Image: " << config.xPixels() << " x " << config.yPixels() << endl;
	cout << "DPI: " << config.dpi << endl;
	cout << "Color type: " << config.colorType << endl;
}
