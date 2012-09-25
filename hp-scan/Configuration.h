#pragma once

#include "Message.h"

class Configuration {
public:
	Configuration();
	Configuration(const Message &msg);

	operator Message() const;

	nat brightness; //From 0x1 to 0xB. Default 0x6
	nat contrast; //From 0x1 to 0xB. Default 0x6
	nat dpi; //75, 100, 150, 200, 300, 600, 1200
	nat colorType; //0x2 = RGB, 0x6 = grey
	nat colorType2; //0x1 = RGB, 0x0 = grey
	nat xMin, xMax;
	nat yMin, yMax;

	nat xPixels() const;
	nat yPixels() const;

	void apply(const Configuration &other);
	void dump();
private:
	bool validSettings;
	Message settings;
};
