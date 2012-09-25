#pragma once

#include "Image.h"

class Output {
public:
	virtual ~Output();

	virtual Image *nextPage() = 0;
	virtual void finish() = 0;
};
