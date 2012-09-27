#pragma once

#include "Output.h"

#include <hpdf.h>
#include <string>

using namespace std;

class PdfOutput : public Output {
public:
	PdfOutput(const string &to);
	virtual ~PdfOutput();

	virtual Image *nextPage();
	virtual void finish();
private:
	string outputTo;
	HPDF_Doc pdf;

	static void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *userData);
};
