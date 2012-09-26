#include "stdafx.h"
#include "Progress.h"

Progress::Progress(ostream &to, const string &message, nat maxProgress) : to(to), message(message), max(maxProgress) {
	update(0);
}

Progress::~Progress() {
	//print(max);
	to << endl;
}


void Progress::update(nat v) {
	to << '\r';
	print(v);
}

void Progress::print(nat v) {
	char tmp[size + 1];

	nat b = (size * v) / max;
	for (nat i = 0; i < size; i++) {
		if (i < b) tmp[i] = '=';
		else if (i > b) tmp[i] = ' ';
		else tmp[i] = '>';
	}

	tmp[size] = 0;

	to << message << " [" << tmp << "]";
}