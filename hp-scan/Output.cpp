#include "stdafx.h"
#include "Output.h"

#include "ImageOutput.h"
#include "PdfOutput.h"
#include "PngImage.h"
#include "BmpImage.h"

#include "QuickFetch.h"

#include <boost/algorithm/string.hpp>

Output::~Output() {}

Output *Output::create(const string &type, const string &path) {
	string t = boost::to_lower_copy(type);
	if (t == "png") return new ImageOutput<PngImage>(path);
	if (t == "bmp") return new ImageOutput<BmpImage>(path);
	if (t == "pdf") return new PdfOutput(path);

	return null;
}

Output *Output::createQuickFetcher(Output *target, const string &tmpName) {
	return new QuickFetch(target, tmpName);
}