#pragma once

#include "Output.h"

#include <string>
#include <sstream>

using namespace std;

template <class T>
class ImageOutput : public Output {
public:
	ImageOutput(const string &path) : path(path), id(0) {};
	virtual ~ImageOutput() {};

	virtual Image *nextPage() {
		return new T(new ofstream(getName(id++).c_str(), ios_base::binary | ios_base::out));
	}

	virtual void finish() {};

private:
	nat id;
	string path;

	string getName(nat id) {
		ostringstream ss;
		ss << path;
		ss << id;
		ss << ".";
		ss << T::extension();
		return ss.str();
	}
};
