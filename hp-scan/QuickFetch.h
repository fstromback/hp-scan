#pragma once

#include "Output.h"

#include <string>

class QuickFetch : public Output {
public:
	QuickFetch(Output *to, const string &tmpName);
	virtual ~QuickFetch();

	virtual Image *nextPage();
	virtual void finish();
private:
	Output *finalTarget;
	string tmpName;
	int numPages;

	ostream *outputFile;

	enum { sInit, sLine, sFinish }; //Operation codes in the output file. Stored as bytes.

	class PageCache : public Image {
	public:
		PageCache(ostream &to);
		virtual ~PageCache();

		virtual void initialize(nat w, nat h, nat dpi);
		virtual void addLine(Color *scanline);
		virtual void finish();
	private:
		nat w, h, dpi;

		ostream &to;
	};
};
