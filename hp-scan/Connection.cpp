#include "stdafx.h"

#include "Connection.h"

#include <boost/asio/buffer.hpp>

Connection::Connection(const string &host, const string &service) : socket(getIoService()) {
	connect(host, service);
}

Connection::~Connection() {
	socket.close();
}

void Connection::connect(const string &host, const string &service) {
	tcp::resolver resolver(getIoService());
	tcp::resolver::query query(host, service);
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;

	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end) {
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error) throw boost::system::system_error(error);
}

boost::asio::io_service &Connection::getIoService() {
	static boost::asio::io_service io_serivice;
	return io_serivice;
}

void Connection::send(const Message &msg) {
	msg.updateHeader();
	size_t s = socket.send(boost::asio::buffer(msg.data, msg.size + Message::headerSize));
	BOOST_ASSERT(s == msg.size + Message::headerSize);
}

void Connection::read(void *to, nat size) {
	byte *at = (byte *)to;
	nat remaining = size;
	while (remaining) {
		size_t s = socket.read_some(boost::asio::buffer(at, remaining));
		remaining -= s;
		at += s;
		if (remaining <= 0) remaining = 0;
	}
}

Message Connection::receive() {
	byte header[Message::headerSize];
	read(header, Message::headerSize);

	Message msg(header);
	read(msg.userData, msg.size);
	return msg;
}

