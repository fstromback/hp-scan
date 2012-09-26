#include "stdafx.h"

#include "Scanner.h"
#include "PngImage.h"
#include "ImageOutput.h"
#include "Commands.h"

#ifdef WIN32
#include <conio.h>
#include <Windows.h>
#endif

using namespace std;



void printUsage(char *name) {
	cout << "Usage:" << endl;
	cout << name << " <printer hostname> [-dpi <dpi>] [-f <format>] [-s <size>] [output name]" << endl;
	cout << endl;
	cout << "-f    : Output format. Currently bmp, png and pdf are supported." << endl;
	cout << "-dpi  : The resolution to scan (in dpi). 75, 100, 150, 200, 300, 600 or 1200." << endl;
	cout << "-s    : The size of the source paper(s). For example: A4." << endl;
	cout << "output name is the name of the outputted file, without extension." << endl;
	cout << endl;
	cout << "Sample usage:" << endl;
	cout << name << " my-printer myScan - Scan at 600 dpi to png files named myScan.png, myScan1.png..." << endl;
	cout << name << " my-printer -dpi 300 -f png myScan - Scan at 300 dpi to png files named myScan.png, myScan1.png..." << endl;
}

#pragma warning (disable : 4996)

int main(int argc, char* argv[]) {
	int result = 0;

	Commands cmd;
	if (!cmd.parseCommandLine(argc, argv)) {
		printUsage(argv[0]);
		result = -1;
	} else {
		cmd.print();

		Output *output = Output::create(cmd.format, cmd.fileName);
		if (output) {
			Scanner scanner(cmd.host(), cmd.port());

			if (scanner.scan(*output)) {
				cout << "Done!" << endl;
				result = 0;
			} else {
				cout << "Failed: " << scanner.getLastError() << endl;
				result = -1;
			}
			delete output;
		} else {
			result = -1;
			cout << "Invalid output format specified: " << cmd.format << endl;
		}

	}

#ifdef WIN32
	getch();
#endif
	return result;
}

