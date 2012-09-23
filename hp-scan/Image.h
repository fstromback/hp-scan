#pragma once


class Image {
public:
	Image();
	virtual ~Image();

	virtual void initialize(nat w, nat h) = 0;
	virtual void addPixel(byte r, byte g, byte b) = 0;
	virtual void flush() = 0;
};
