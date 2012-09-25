#include "stdafx.h"
#include "Configuration.h"

#include <iostream>

using namespace std;

Configuration::Configuration() : validSettings(false) {}

Configuration::Configuration(const Message &msg) : validSettings(true), settings(msg) {
	brightness = msg.getNat(1 * 4);
	contrast = msg.getNat(2 * 4);
	dpi = msg.getNat(3 * 4);
	colorType = msg.getNat(15 * 4);
	colorType2 = msg.getNat(24 * 4);
	xMin = msg.getNat(17 * 4);
	xMax = msg.getNat(19 * 4);
	yMin = msg.getNat(16 * 4);
	yMax = msg.getNat(18 * 4);
}

void Configuration::apply(const Configuration &other) {
	brightness = other.brightness;
	contrast = other.contrast;
	dpi = other.dpi;
	colorType = other.colorType;
	colorType2 = other.colorType2;
	xMin = other.xMin;
	xMax = other.xMax;
	yMin = other.yMin;
	yMax = other.yMax;
}

Configuration::operator Message() const {
	BOOST_ASSERT(validSettings);

	Message r = settings;
	r.setNat(1 * 4, brightness);
	r.setNat(2 * 4, contrast);
	r.setNat(3 * 4, dpi);
	r.setNat(15 * 4, colorType);
	r.setNat(24 * 4, colorType2);
	r.setNat(17 * 4, xMin);
	r.setNat(19 * 4, xMax);
	r.setNat(16 * 4, yMin);
	r.setNat(18 * 4, yMax);
	//r.setNat(21 * 4, xMin);
	//r.setNat(23 * 4, xMax);
	//r.setNat(20 * 4, yMin);
	//r.setNat(22 * 4, yMax);
	return r;
}

nat round2(double x) {
	return (x >= 0.0) ? (nat)(x+0.5) : (nat)(x-0.5);
}

nat Configuration::xPixels() const {
	//const double MAX_X_S = 220.0;
	nat dx = xMax - xMin;
	return (dx * dpi) / 100;
	/*
	switch (dpi) {
			case 75:
				return round2((dx / ((double) MAX_X_S)) * 640);
			case 100:
				return round2((dx / ((double) MAX_X_S)) * 848);
			case 150:
				return round2((dx / ((double) MAX_X_S)) * 1264);
			case 200:
				return round2((dx / ((double) MAX_X_S)) * 1696);
			case 300:
				return round2((dx / ((double) MAX_X_S)) * 2528);
			case 600:
				return round2((dx / ((double) MAX_X_S)) * 5088);
			case 1200:
				return round2((dx / ((double) MAX_X_S)) * 10208);
		}

		return 0;*/
	
}

nat Configuration::yPixels() const {
	//const double MAX_Y_S = 330;
	nat dy = yMax - yMin;
	return (dy * dpi) / 100;
	/*
	switch (dpi) {
			case 75:
				return round2((dy / ((double) MAX_Y_S)) * 880);
			case 100:
				return round2((dy / ((double) MAX_Y_S)) * 1180);
			case 150:
				return round2((dy / ((double) MAX_Y_S)) * 1775);
			case 200:
				return round2((dy / ((double) MAX_Y_S)) * 2351);
			case 300:
				return round2((dy / ((double) MAX_Y_S)) * 3510);
			case 600:
				return round2((dy / ((double) MAX_Y_S)) * 7020);
			case 1200:
				return round2((dy / ((double) MAX_Y_S)) * 14025);
		}

		return 0;*/
	
}

void Configuration::dump() {
	cout << dec;
	cout << "Config: " << endl;
	cout << "Size: " << xPixels() << " x " << yPixels() << endl;
	cout << "DPI: " << dpi << endl;
	cout << "ColorMode: " << colorType << ", " << colorType2 << endl;
}