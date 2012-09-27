#include "stdafx.h"
#include "PdfImage.h"

#include <fstream>


PdfImage::PdfImage(HPDF_Doc doc, HPDF_Page page, const string &tmpName) 
: doc(doc), page(page), pngImage(null), tmpName(tmpName) {}


PdfImage::~PdfImage() {
	if (pngImage) delete pngImage;
}

void PdfImage::initialize(nat w, nat h, nat dpi) {
	pngImage = new PngImage(new ofstream(tmpName.c_str(), ios_base::binary | ios_base::out));
	pngImage->initialize(w, h, dpi);

	this->w = w;
	this->h = h;
	this->dpi = dpi;

	HPDF_Page_SetWidth(page, 100.0f * HPDF_REAL(w) / float(dpi));
	HPDF_Page_SetHeight(page, 100.0f * HPDF_REAL(h) / float(dpi));
}

float PdfImage::pageWidth() const {
	return 100.0f * float(w) / float(dpi);
}

float PdfImage::pageHeight() const {
	return 100.0f * float(h) / float(dpi);
}

void PdfImage::addLine(Color *scanline) {
	pngImage->addLine(scanline);
}

void PdfImage::finish() {
	pngImage->finish();
	delete pngImage;
	pngImage = null;

	HPDF_Image image = HPDF_LoadPngImageFromFile(doc, tmpName.c_str());
	HPDF_Page_DrawImage(page, image, 0, 0, 100.0f * HPDF_REAL(w) / float(dpi), 100.0f * HPDF_REAL(h) / float(dpi));
}

