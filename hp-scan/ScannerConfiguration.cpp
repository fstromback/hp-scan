#include "stdafx.h"
#include "ScannerConfiguration.h"

#include <iostream>

using namespace std;

ScannerConfiguration::ScannerConfiguration() : validSettings(false) {}

ScannerConfiguration::ScannerConfiguration(const Message &msg) : validSettings(true), settings(msg) {
	brightness = msg.getNat(1 * 4);
	contrast = msg.getNat(2 * 4);
	dpi = msg.getNat(3 * 4);
	colorType = msg.getNat(15 * 4);
	colorType2 = msg.getNat(24 * 4);
	pos.left = msg.getNat(17 * 4);
	pos.right = msg.getNat(19 * 4);
	pos.top = msg.getNat(16 * 4);
	pos.bottom = msg.getNat(18 * 4);
	defaultPos.left = msg.getNat(21 * 4);
	defaultPos.right = msg.getNat(23 * 4);
	defaultPos.top = msg.getNat(20 * 4);
	defaultPos.bottom = msg.getNat(22 * 4);
}

void ScannerConfiguration::apply(const ScannerConfiguration &other) {
	brightness = other.brightness;
	contrast = other.contrast;
	dpi = other.dpi;
	colorType = other.colorType;
	colorType2 = other.colorType2;
	pos = other.pos;
	defaultPos = other.defaultPos;
}

void ScannerConfiguration::apply(const Settings &from) {
	dpi = from.dpi;
	if (from.scanRect.width() > 0 && from.scanRect.height() > 0) pos = from.scanRect;
}

ScannerConfiguration::operator Message() const {
	BOOST_ASSERT(validSettings);

	Message r = settings;
	r.setNat(1 * 4, brightness);
	r.setNat(2 * 4, contrast);
	r.setNat(3 * 4, dpi);
	r.setNat(15 * 4, colorType);
	r.setNat(24 * 4, colorType2);
	r.setNat(17 * 4, pos.left);
	r.setNat(19 * 4, pos.right);
	r.setNat(16 * 4, pos.top);
	r.setNat(18 * 4, pos.bottom);
/*
	r.setNat(21 * 4, defaultPos.left);
	r.setNat(23 * 4, defaultPos.right);
	r.setNat(20 * 4, defaultPos.top);
	r.setNat(22 * 4, defaultPos.bottom);*/

	return r;
}

nat round2(double x) {
	return (x >= 0.0) ? (nat)(x+0.5) : (nat)(x-0.5);
}

nat ScannerConfiguration::xPixels() const {
	return (pos.width() * dpi) / 100;
}

nat ScannerConfiguration::yPixels() const {
	return (pos.height() * dpi) / 100;
}

void ScannerConfiguration::dump() {
	cout << dec;
	cout << "Config: " << endl;
	cout << "Size: " << xPixels() << " x " << yPixels() << endl;
	cout << "DPI: " << dpi << endl;
	cout << "ColorMode: " << colorType << ", " << colorType2 << endl;
}