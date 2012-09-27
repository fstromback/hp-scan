#include "stdafx.h"
#include "PdfOutput.h"
#include "PdfImage.h"

PdfOutput::PdfOutput(const string &path) : outputTo(path + ".pdf") {
	pdf = HPDF_New(error_handler, null);
	HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
	HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_USE_OUTLINE);	
}

PdfOutput::~PdfOutput(void) {
	HPDF_Free(pdf);
}

Image *PdfOutput::nextPage() {
	HPDF_Page page = HPDF_AddPage(pdf);
	return new PdfImage(pdf, page, outputTo);
}

void PdfOutput::finish() {	
	HPDF_SaveToFile(pdf, outputTo.c_str());
}

void PdfOutput::error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *userData) {
	cout << "Error in libharu: " << error_no << ", " << detail_no << endl;
}