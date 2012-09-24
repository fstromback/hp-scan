#include "stdafx.h"

#include "Connection.h"
#include "Image.h"
#include "BmpImage.h"
#include "PngImage.h"
#include "ImageReader.h"
#include "Configuration.h"

#include <iostream>
#include <fstream>
#include <string>

#include <conio.h>

using namespace std;

bool initialize(Connection &conn) {
	Message m;

	conn.send(Message::reset());
	conn.send(Message::status());
	m = conn.receive();
	if (!m.ok()) return false;

	return true;
}

void cleanUp(Connection &conn) {
	conn.send(Message::reset());
}

bool doScan(Connection &conn) {

	conn.send(Message::askAdf());
	Message m = conn.receive();

	bool useAdf = m.getHeader().r1 == 1;
	cout << (useAdf ? "Using adf" : "Not using adf") << endl;

	conn.send(Message::getConfig());
	Message configMsg = conn.receive();
	if (!configMsg.ok()) return false;
	Configuration config(configMsg);

	config.dpi = 75;
	config.colorType2 = 1;
	
	//BmpImage *image = new BmpImage(new std::ofstream("img.bmp", std::ios::binary | std::ios::out));
	PngImage *image = new PngImage(new std::ofstream("img.png", std::ios::binary | std::ios::out));
	//ImageReader img(config, new RandomAccessImage(bmpImage));
	ImageReader img(config, image);

	Message::setConfig(config).dump();
	conn.send(Message::setConfig(config));
	m = conn.receive();
	if (!m.ok()) return false;
	m.dump();

	conn.send(Message::startScan());
	m = conn.receive();
	do {
		//0xA, 0xB seems to be sent before each page
		while (m.ok()) {
			bool atEnd = false;
			switch (m.getHeader().type) {
				case 0xE:
					atEnd = true;
					m.dump();
					break;
				case 0x5:
					img.addData(m);
					break;
				case 0x4:
					//restart or abort sent when misfeed?
					return false;
				default:
					cout << "Unknown message:" << hex << m.getHeader().type << endl;
					m.dump();
					break;
			}
			
			if (atEnd) break;
			m = conn.receive();
		}
		if (!m.ok()) return false;
		img.dump();

		m = conn.receive();
		if (!m.ok()) return false;
		m.dump();
		//Should be a 0xD

		if (useAdf) {
			//Sends 0xA, 0xB between pages. Sends 0xC after the last page.
			m = conn.receive();
			if (!m.ok()) return false;

			if (m.getHeader().type == 0xC) useAdf = false; //Done!
		}
	} while (useAdf);

	return true;
}

bool doScan(string addr) {
	Connection conn(addr);

	if (!initialize(conn)) return false;
	bool result = doScan(conn);
	cleanUp(conn);
	return result;
}

#pragma warning (disable : 4996)

int main(int argc, char* argv[]) {


	if (!doScan("192.168.0.108")) cout << "Failed..." << endl;
	else cout << "Done!" << endl;

	getch();
	return 0;
}

