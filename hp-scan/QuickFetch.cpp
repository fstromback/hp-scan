#include "stdafx.h"
#include "QuickFetch.h"

#include "Progress.h"

#include <fstream>

QuickFetch::QuickFetch(Output *to, const string &tmpName) : finalTarget(to), tmpName(tmpName + ".tmp"), numPages(0), outputFile(null) {}

QuickFetch::~QuickFetch() {
	delete finalTarget;
	delete outputFile;
}

Image *QuickFetch::nextPage() {
	if (outputFile == null) {
		outputFile = new ofstream(tmpName.c_str(), ios::binary | ios::out);
	}

	numPages++;
	return new PageCache(*outputFile);
}

void QuickFetch::finish() {
	delete outputFile; outputFile = null;

	ifstream input(tmpName.c_str(), ios::binary | ios::in);

	Progress progress(cout, "Saving scan", numPages);

	Color *tmpColors = null;
	nat colorsWidth = 0;
	Image *page = null;
	nat width = 0;
	nat height = 0;
	nat dpi = 0;
	for (int i = 0; i < numPages;) {
		char op;
		input.read(&op, sizeof(char));
		switch (op) {
			case sInit:
				input.read((char *)&width, sizeof(nat));
				input.read((char *)&height, sizeof(nat));
				input.read((char *)&dpi, sizeof(nat));

				delete page;
				page = finalTarget->nextPage();
				page->initialize(width, height, dpi);
				break;
			case sLine:
				if (colorsWidth != width) {
					delete tmpColors;
					tmpColors = new Color[width];
					colorsWidth = width;
				}
				input.read((char *)tmpColors, sizeof(Color) * width);
				page->addLine(tmpColors);
				break;
			case sFinish:
				page->finish();
				i++;
				progress.update(i);
				break;
		}
	}
	delete tmpColors;
	delete page;

	input.close();

	finalTarget->finish();

	remove(tmpName.c_str());
}

//////////////////////////////////////////////////////////////////////////
// The cache
//////////////////////////////////////////////////////////////////////////

QuickFetch::PageCache::PageCache(ostream &to) : w(0), h(0), dpi(0), to(to) {}

QuickFetch::PageCache::~PageCache() {}

void QuickFetch::PageCache::initialize(nat w, nat h, nat dpi) {
	this->w = w;
	this->h = h;
	this->dpi = dpi;

	char op = sInit;
	to.write(&op, sizeof(char));
	to.write((char *)&w, sizeof(nat));
	to.write((char *)&h, sizeof(nat));
	to.write((char *)&dpi, sizeof(nat));
}

void QuickFetch::PageCache::addLine(Color *scanline) {
	char op = sLine;
	to.write(&op, sizeof(char));
	to.write((char *)scanline, sizeof(Color) * w);
}

void QuickFetch::PageCache::finish() {
	char op = sFinish;
	to.write(&op, sizeof(char));
}