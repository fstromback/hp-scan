#pragma once

#include "Image.h"
#include "PngImage.h"

#include <hpdf.h>

class PdfImage : public Image {
public:
	PdfImage(HPDF_Doc doc, HPDF_Page page, const string &tmpName);
	virtual ~PdfImage();

	virtual void initialize(nat w, nat h, nat dpi);
	virtual void addLine(Color *scanline);
	virtual void finish();
private:
	HPDF_Doc doc;
	HPDF_Page page;
	PngImage *pngImage;
	string tmpName;
	nat w, h, dpi;

	float pageWidth() const;
	float pageHeight() const;
};
