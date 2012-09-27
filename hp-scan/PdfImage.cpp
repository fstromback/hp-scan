#include "stdafx.h"
#include "PdfImage.h"

#include <fstream>

PdfImage::PdfImage(HPDF_Doc doc, HPDF_Page page, const string &name) : doc(doc), page(page), pngImage(new ofstream(getTmpFile(name).c_str(), ios_base::binary | ios_base::out)), name(name) {}


PdfImage::~PdfImage() {}

void PdfImage::initialize(nat w, nat h) {
	pngImage.initialize(w, h);

	this->w = w;
	this->h = h;

	HPDF_Page_SetWidth(page, HPDF_REAL(w));
	HPDF_Page_SetHeight(page, HPDF_REAL(h));
}

void PdfImage::addLine(Color *scanline) {
	pngImage.addLine(scanline);
}

void PdfImage::finish() {
	pngImage.finish();

	HPDF_Image image = HPDF_LoadPngImageFromFile(doc, getTmpFile(name).c_str());
	HPDF_Page_DrawImage(page, image, 0, 0, HPDF_REAL(w), HPDF_REAL(h));
}

string PdfImage::getTmpFile(const string &name) {
	return name + ".png";
}