#pragma once

#include "Connection.h"
#include "Configuration.h"
#include "Output.h"
#include "ImageReader.h"

#include <iostream>
#include <string>

using namespace std;

class Scanner {
public:
	Scanner(const string &host, ostream &logTo = cout);
	
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

	bool getConfiguration(Configuration &to);
	bool setConfiguration(Configuration &conf);

	bool scanPage(ImageReader &page);

	void reset();

	bool fail(const string &msg);
};
