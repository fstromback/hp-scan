#pragma once

#include "Configuration.h"
#include "Image.h"
#include "RandomAccessImage.h"

#include <fstream>
#include <queue>

class ImageReader {
public:
	ImageReader(Configuration &description, Image *to);
	~ImageReader();

	void addData(Message &data);

	void dump();
private:
	ImageReader(const ImageReader &o);
	Configuration config;
	
	RandomAccessImage buffer;
	Image *to;

	enum { headerSize = 24 };
};
