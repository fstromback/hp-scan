#include "stdafx.h"
#include "Commands.h"

Commands::Commands() : dpi(600), format("png"), size("A4") {}


bool Commands::parseCommandLine(int argc, char *argv[]) {
	if (argc < 3) return false;
	printer = argv[1];

	for (int i = 2; i < argc; i++) {
		string a = argv[i];
		if (a == "") {
		} else if (a[0] == '-') {
			if (++i >= argc) return false;
			if (a == "-f") {
				format = argv[i];
			} else if (a == "-dpi") {
				dpi = atoi(argv[i]);
			} else if (a == "-s") {
				size = argv[i];
			} else {
				return false;
			}
		} else if (i + 1 == argc) {
			fileName = a;
			return true;
		} else {
			return false;
		}
	}
	return false;
}

string Commands::host() const {
	nat i = printer.rfind(':');
	if (i == string::npos) return printer;
	return printer.substr(0, i);
}

string Commands::port() const {
	nat i = printer.rfind(':');
	if (i == string::npos) return "8290";
	return printer.substr(i + 1);
}

void Commands::print() const {
	cout << "Scanning from " << host() << " in " << dpi << " dpi to " << format << "..." << endl;
}