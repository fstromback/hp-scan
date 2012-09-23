#pragma once

#include "Image.h"

class RandomAccessImage {
public:
	RandomAccessImage(nat w, nat h, nat d);
	virtual ~RandomAccessImage();

	
	inline byte &at(nat x, nat y, nat ch) { return data[(x + y * w) * d + ch]; };
	void flush(Image *to);
private:
	byte *data;
	nat w, h, d;
};
