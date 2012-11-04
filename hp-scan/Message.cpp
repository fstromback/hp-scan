#include "stdafx.h"

#include "Message.h"

#include <boost/asio.hpp>
#include <iomanip>
#include <iostream>

using namespace std;

Message::Message(nat size, nat type) {
	init(size);
	initializeHeader(type);
}

Message::Message(const Message &other) : header(other.header) {
	init(other.size);
	memcpy(data, other.data, size + headerSize);
}

Message::Message(byte *data) {
	nat *incomingData = (nat *)data;
	nat *headerData = (nat *)&header;
	for (int i = 0; i < headerWords; i++) {
		headerData[i] = ntohl(incomingData[i]);
	}
	init(header.size);
}

Message::~Message() {
	if (data) delete data;
}

Message &Message::operator =(const Message &other) {
	if (data) delete data;
	init(other.size);
	header = other.header;
	memcpy(data, other.data, size + headerSize);
	return *this;
}

void Message::initializeHeader(nat type /* = 0 */) {
	memset(&header, 0, sizeof(Header));
	header.magicNumber = magicNumber;
	header.type = type;
	header.size = size;
}

bool Message::validHeader(const Header &h) {
	if (h.magicNumber != magicNumber) return false;
	return true;
}

bool Message::valid() const {
	if (!validHeader(header)) return false;
	return header.size == size;
}

bool Message::ok() const {
	if (!valid()) return false;
	return header.status == 0;
}

void Message::init(nat size) {
	BOOST_ASSERT_MSG(headerSize == sizeof(Header), "Error: The size of the header does not match the expected header size. Check the size of nat.");

	this->size = size;
	data = new byte[size + headerSize];
	userData = &data[headerSize];
}

void Message::updateHeader() const {
	nat *headerData = (nat *)&header;
	nat *target = (nat *)data;
	for (int i = 0; i < headerWords; i++) {
		target[i] = htonl(headerData[i]);
	}
}

void Message::dump(bool large) {
	cout << dec;
	cout << "Message:" << header.type << endl;
	cout << "Length: " << getSize() << " bytes";

	if (large) {
		for (nat i = 0; i < getSize(); i++) {
			if (i % 8 == 0) cout << endl;
			if (i % 8 == 4) cout << " ";
			cout << hex << fixed << setfill('0') << setw(2) << (int)userData[i] << " ";
		}
	}
	cout << endl;
}

nat Message::getNat(nat at) const {
	return ntohl(*((nat *)&userData[at])); 
}

void Message::setNat(nat at, nat d) {
	*((nat *)&userData[at]) = htonl(d);
}

Message Message::reset() {
	return Message(0, 0x15);
}

Message Message::status() {
	return Message(0, 0x0);
}

Message Message::getConfig() {
	return Message(0, 0x6);
}

Message Message::setConfig(const Message &from) {
	Message m = from;
	m.setType(0x7);
	return m;
}

Message Message::startScan() {
	return Message(0, 0x2);
}

Message Message::askAdf() {
	return Message(0, 0xF);
}