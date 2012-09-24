#pragma once

class Color {
public:
	Color();
	Color(byte r, byte g, byte b);

	byte &operator[](nat item);

	byte r, g, b;
};
