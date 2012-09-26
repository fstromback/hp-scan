#pragma once

#include <string>

using namespace std;

class Commands {
public:
	Commands();

	string printer;
	string fileName;
	string format;
	nat dpi;
	string size;

	bool parseCommandLine(int argc, char *argv[]);
	string host() const;
	string port() const;
	void print() const;
};
