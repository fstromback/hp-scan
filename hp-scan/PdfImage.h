#pragma once

#include "Image.h"
#include "PngImage.h"

#include <hpdf.h>

class PdfImage : public Image {
public:
	PdfImage(HPDF_Doc doc, HPDF_Page page, const string &name);
	virtual ~PdfImage();

	virtual void initialize(nat w, nat h);
	virtual void addLine(Color *scanline);
	virtual void finish();
private:
	HPDF_Doc doc;
	HPDF_Page page;
	PngImage pngImage;
	string name;
	nat w, h;

	static string getTmpFile(const string &name);
};
