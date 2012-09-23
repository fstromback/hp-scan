#include "stdafx.h"

#include "Connection.h"
#include "Image.h"
#include "BmpImage.h"
#include "ImageReader.h"
#include "Configuration.h"
#include "RandomAccessImage.h"

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

	conn.send(Message::getConfig());
	Message configMsg = conn.receive();
	if (!configMsg.ok()) return false;
	Configuration config(configMsg);

	config.dpi = 100;
	config.colorType2 = 1;
	
	BmpImage *bmpImage = new BmpImage(new std::ofstream("img.bmp", std::ios::binary | std::ios::out));
	//ImageReader img(config, new RandomAccessImage(bmpImage));
	ImageReader img(config, bmpImage);

	Message::setConfig(config).dump();
	conn.send(Message::setConfig(config));
	Message m = conn.receive();
	if (!m.ok()) return false;
	m.dump();

	conn.send(Message::startScan());
	m = conn.receive();
	while (m.ok()) {
		bool atEnd = false;
		switch (m.getHeader().type) {
			case 0xE:
				atEnd = true;
				break;
			case 0x5:
				img.addData(m);
				break;
			default:
				cout << "Unknown message:" << endl;
				m.dump();
				break;
		}
		
		if (atEnd) break;
		m = conn.receive();
	}
	if (!m.ok()) return false;
	img.dump();

	conn.send(Message(0x3));
	m = conn.receive();
	if (!m.ok()) return false;
	m.dump();

	conn.send(Message(0x3));
	m = conn.receive();
	if (!m.ok()) return false;
	m.dump();

	//Needed??
	//conn.send(Message(0x1));
	//m = conn.receive();
	//if (!m.ok()) return false;
	//m.dump();

	return true;
}

bool doScan(string addr) {
	Connection conn(addr);

	if (!initialize(conn)) return false;
	bool result = doScan(conn);
	cleanUp(conn);
	return result;
}

int main(int argc, char* argv[]) {


/*
	BmpImage img(new std::ofstream("img.bmp", std::ios::binary | std::ios::out));
	img.initialize(11, 10);
	for (int x = 0; x < 11; x++) {
		for (int y = 0; y < 10; y++) {
			img.addPixel(250, 50, 50);
		}
	}

	return 0;*/



	if (!doScan("192.168.0.108")) cout << "Failed..." << endl;
	else cout << "Done!" << endl;

	getch();
	return 0;
}

