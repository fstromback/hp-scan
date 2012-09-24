#include "stdafx.h"
#include "Color.h"

Color::Color() : r(0), g(0), b(0) {}

Color::Color(byte r, byte g, byte b) : r(r), g(g), b(b) {}

byte &Color::operator[](nat item) {
	switch (item) {
		case 0:
			return b;
		case 1:
			return r;
		case 2:
			return g;
	}
	return r;
}