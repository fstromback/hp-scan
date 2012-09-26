#include "stdafx.h"
#include "Scanner.h"

Scanner::Scanner(const string &host, const string &port, ostream &logTo /* = cout */) : logTo(logTo), connection(host, port) {}

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

bool Scanner::getConfiguration(ScannerConfiguration &to) {
	connection.send(Message::getConfig());
	Message m = connection.receive();
	if (!m.ok()) return false;
	to = ScannerConfiguration(m);
	return true;
}

bool Scanner::setConfiguration(ScannerConfiguration &conf) {
	connection.send(Message::setConfig(conf));
	Message m = connection.receive();
	return m.ok();
}

string Scanner::getPageString(nat id) {
	stringstream ss;
	ss << "Scanning page " << id;
	return ss.str();
}

string Scanner::getBadFormatString(nat id) {
	stringstream ss;
	ss << "Bad input type: " << id;
	return ss.str();
}

bool Scanner::scanPage(ImageReader &page, Progress &progress) {
	Message m = connection.receive();
	nat status = 0;
	while (m.ok()) {
		if (m.getHeader().type == 0xE) break;

		switch (m.getHeader().type) {
			case 0x5:
				status = page.addData(m, progress);
				if (status != 0) return fail(getBadFormatString(status));
				break;
			case 0x4:
				return fail("ADF paper mispick.");
			case 0xA:
			case 0xB:
				//Sent between pages.
				break;
			default:
				//logTo << "Unknown message: " << hex << m.getHeader().type << endl;
				break;
		}

		m = connection.receive();
	}
	return true;
}

bool Scanner::scanInternal(Output &to) {
	bool useAdf = usingAdf();
	logTo << (useAdf ? "Using adf" : "Not using adf") << endl;

	if (useAdf) {
		if (settings.dpi < 100) logTo << "Warning: Scanning with the ADF in low resolutions will cause invalid images." << endl;
	}

	ScannerConfiguration c;
	if (!getConfiguration(c)) return fail("Failed to get the configuration.");

	c.colorType = 2;
	c.colorType2 = 1;
	//Apply settings here
	c.apply(settings);

	if (!setConfiguration(c)) return fail("Failed to set the configuration.");

	connection.send(Message(0, 0x8));
	if (!connection.receive().ok()) return fail("Unknown error.");

	connection.send(Message::startScan());

	nat pageNumber = 0;

	bool morePages = true;
	while (morePages) {
		{
			Progress progress(logTo, getPageString(++pageNumber), c.yPixels() * 3);
			ImageReader reader(c, to.nextPage());
			if (!scanPage(reader, progress)) return false;
		}

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