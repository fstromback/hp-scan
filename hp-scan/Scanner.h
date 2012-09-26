#pragma once

#include "Connection.h"
#include "ScannerConfiguration.h"
#include "Output.h"
#include "ImageReader.h"
#include "Settings.h"

#include <iostream>
#include <string>

using namespace std;

class Scanner {
public:
	Scanner(const string &host, const string &port = "8290", ostream &logTo = cout);
	
	Settings settings;

	bool scan(Output &to);
	
	const string &getLastError() const { return lastError; };
private:
	ostream &logTo;
	string lastError;
	Connection connection;

	bool initializeScan();
	void cleanUpScan();

	bool scanInternal(Output &to);
	bool usingAdf(); //Public? Requires reset?

	bool getConfiguration(ScannerConfiguration &to);
	bool setConfiguration(ScannerConfiguration &conf);

	bool scanPage(ImageReader &page, Progress &progress);

	void reset();

	bool fail(const string &msg);
	string getPageString(nat id);
	string getBadFormatString(nat id);
};
