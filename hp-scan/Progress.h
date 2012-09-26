#pragma once

#include <iostream>
#include <string>

using namespace std;

class Progress {
public:
	Progress(ostream &to, const string &message, nat maxProgress);
	~Progress();

	void update(nat v);
private:
	ostream &to;
	string message;
	nat max;

	enum { size = 30 };

	void print(nat v);
};
