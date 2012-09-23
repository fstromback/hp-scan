#include "stdafx.h"
#include "RandomAccessImage.h"

RandomAccessImage::RandomAccessImage(nat w, nat h, nat d) : w(w), h(h), d(d) {
	data = new byte[w * h * d];
}

RandomAccessImage::~RandomAccessImage() {
	if (data) delete []data;
}

void RandomAccessImage::flush(Image *to) {
	for (nat y = h; y > 0; y--) {
		for (nat x = 0; x < w; x++) {
			to->addPixel(at(x, y - 1, 1), at(x, y - 1, 2), at(x, y - 1, 0)); //TODO: Support monochrome as well!
		}
	}
	to->flush();
}