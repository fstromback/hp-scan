#pragma once

#include "ScannerConfiguration.h"
#include "Image.h"
#include "Progress.h"

#include <fstream>
#include <queue>

class ImageReader {
public:
	ImageReader(ScannerConfiguration &description, Image *to);
	~ImageReader();

	nat addData(Message &data, Progress &p);

	void dump();
private:
	ImageReader(const ImageReader &o);
	ScannerConfiguration config;
	
	Image *to;
	Color *scanline;
	nat flushedLines;

	enum { headerSize = 24 };
	void addScanline(const byte *data, nat width, nat channel);
	void addReverseScanline(const byte *data, nat width, nat channel);
	void flushScanline();
	void fillImage();
};
