#pragma once

#include "Message.h"
#include "Rect.h"
#include "Settings.h"

class ScannerConfiguration {
public:
	ScannerConfiguration();
	ScannerConfiguration(const Message &msg);

	operator Message() const;

	nat brightness; //From 0x1 to 0xB. Default 0x6
	nat contrast; //From 0x1 to 0xB. Default 0x6
	nat dpi; //75, 100, 150, 200, 300, 600, 1200
	nat colorType; //0x2 = RGB, 0x6 = grey
	nat colorType2; //0x1 = RGB, 0x0 = grey

	Rect pos;
	Rect defaultPos;

	nat xPixels() const;
	nat yPixels() const;

	void apply(const Settings &from);
	void apply(const ScannerConfiguration &other);
	void dump();
private:
	bool validSettings;
	Message settings;
};
