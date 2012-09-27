#include "stdafx.h"
#include "PdfOutput.h"
#include "PdfImage.h"

#include <sstream>

PdfOutput::PdfOutput(const string &path) : outputTo(path + ".pdf"), tmpId(0) {
	pdf = HPDF_New(error_handler, null);
	HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
	HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_USE_OUTLINE);	
}

PdfOutput::~PdfOutput() {
	HPDF_Free(pdf);

	while (tmpList.size() > 0) {
		remove(tmpList.front().c_str());
		tmpList.pop_front();
	}
}

Image *PdfOutput::nextPage() {
	HPDF_Page page = HPDF_AddPage(pdf);
	return new PdfImage(pdf, page, getTmpName());
}

string PdfOutput::getTmpName() {
	stringstream t;
	t << outputTo << "." << tmpId++;
	tmpList.push_back(t.str());
	return t.str();
}

void PdfOutput::finish() {	
	HPDF_SaveToFile(pdf, outputTo.c_str());
}

void PdfOutput::error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *userData) {
	cout << "Error in libharu: " << error_no << ", " << detail_no << endl;
}