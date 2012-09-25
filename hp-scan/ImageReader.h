#pragma once

#include "Configuration.h"
#include "Image.h"

#include <fstream>
#include <queue>

class ImageReader {
public:
	ImageReader(Configuration &description, Image *to);
	~ImageReader();

	bool addData(Message &data);

	void dump();
private:
	ImageReader(const ImageReader &o);
	Configuration config;
	
	Image *to;
	Color *scanline;
	nat flushedLines;

	enum { headerSize = 24 };
	void addScanline(const byte *data, nat width, nat channel);
	void addReverseScanline(const byte *data, nat width, nat channel);
	void flushScanline();
	void fillImage();
};
