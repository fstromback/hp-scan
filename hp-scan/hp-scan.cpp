#include "stdafx.h"

#include "Scanner.h"
#include "PngImage.h"
#include "ImageOutput.h"

#include <conio.h>

using namespace std;

#pragma warning (disable : 4996)

int main(int argc, char* argv[]) {

	Scanner scanner("192.168.0.108");

	ImageOutput<PngImage> output("output2_");
	if (scanner.scan(output)) cout << "Done!" << endl;
	else cout << "Failed: " << scanner.getLastError() << endl;

	getch();
	return 0;
}

