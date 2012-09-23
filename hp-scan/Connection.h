#pragma once

#include "Message.h"

#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace std;

class Connection {
public:
	Connection(const string &host, const string &service = "8290");
	~Connection();

	void send(const Message &msg);
	Message receive();
private:
	tcp::socket socket;

	void connect(const string &host, const string &service);
	static boost::asio::io_service &getIoService();

	void read(void *to, nat size);
};
