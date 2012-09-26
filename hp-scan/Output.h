#pragma once

#include "Image.h"
#include <string>

using namespace std;

class Output {
public:
	virtual ~Output();

	virtual Image *nextPage() = 0;
	virtual void finish() = 0;

	static Output *create(const string &type, const string &path);
};
