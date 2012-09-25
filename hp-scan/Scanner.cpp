#include "stdafx.h"
#include "Scanner.h"

Scanner::Scanner(const string &host, ostream &logTo /* = cout */) : logTo(logTo), connection(host) {}

bool Scanner::initializeScan() {
	Message m;

	connection.send(Message::reset());
	connection.send(Message::status());
	m = connection.receive();

	return m.ok();
}

void Scanner::cleanUpScan() {
	reset();
}

void Scanner::reset() {
	connection.send(Message::reset());
}

bool Scanner::scan(Output &to) {
	if (!initializeScan()) return false;

	bool result = scanInternal(to);
	to.finish();

	cleanUpScan();

	return result;
}

bool Scanner::usingAdf() {
	connection.send(Message::askAdf());
	Message m = connection.receive();

	return m.getHeader().r1 == 1;
}

bool Scanner::getConfiguration(Configuration &to) {
	connection.send(Message::getConfig());
	Message m = connection.receive();
	if (!m.ok()) return false;
	to = Configuration(m);
	return true;
}

bool Scanner::setConfiguration(Configuration &conf) {
	connection.send(Message::setConfig(conf));
	Message m = connection.receive();
	return m.ok();
}

bool Scanner::scanPage(ImageReader &page) {
	Message m = connection.receive();
	while (m.ok()) {
		if (m.getHeader().type == 0xE) break;

		switch (m.getHeader().type) {
			case 0x5:
				if (!page.addData(m)) return fail("Invalid data occurred");
				break;
			case 0x4:
				return fail("ADF paper misfeed.");
			case 0xA:
			case 0xB:
				//Sent between pages.
				break;
			default:
				logTo << "Unknown message: " << hex << m.getHeader().type << endl;
				break;
		}

		m = connection.receive();
	}
	return true;
}

bool Scanner::scanInternal(Output &to) {
	bool useAdf = usingAdf();
	logTo << (useAdf ? "Using adf" : "Not using adf") << endl;

	Configuration c;
	if (!getConfiguration(c)) return fail("Failed to get the configuration.");

	c.dpi = 75;
	c.colorType = 2;
	c.colorType2 = 1;
	//Apply settings here

	if (!setConfiguration(c)) return fail("Failed to set the configuration.");

	connection.send(Message::startScan());

	nat pageNumber = 0;

	bool morePages = true;
	while (morePages) {
		logTo << "Scanning page " << ++pageNumber << "..." << endl;

		ImageReader reader(c, to.nextPage());
		if (!scanPage(reader)) return false;

		Message m = connection.receive(); //Should be a 0xD

		if (useAdf) {
			m = connection.receive();
			if (!m.ok()) return fail("Invalid data");

			if (m.getHeader().type == 0xC) morePages = false;
		} else {
			morePages = false;
		}
	}

	return true;
}

bool Scanner::fail(const string &msg) {
	lastError = msg;
	return false;
}