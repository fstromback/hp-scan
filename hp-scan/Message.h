#pragma once

class Connection;

class Message {
public:
	Message(nat size = 0, nat type = 0); //Header size not included!
	Message(const Message &other);
	~Message();

	Message &operator =(const Message &other);

	struct Header {
		nat magicNumber;
		nat type;
		nat r1, r2; //reserved
		nat status;
		nat size;
		nat r3, r4; //reserved
	};

	bool valid() const;
	bool ok() const;
	inline const Header &getHeader() const { return header; };
	inline nat getSize() const { return size; };
	inline void setType(nat type) { header.type = type; };

	nat getNat(nat at) const;
	void setNat(nat at, nat data);
	const byte *getData() const { return userData; };

	void dump(bool large = true);

	friend class Connection;
private:
	Message(byte *headerData);

	enum { headerSize = 32, headerWords = headerSize / 4, magicNumber = 0x41535001 };

	Header header;

	byte *data; //The entire data chunk
	byte *userData;
	nat size;

	void init(nat size);
	void initializeHeader(nat type = 0);
	static bool validHeader(const Header &h);
	void updateHeader() const;
public:
	static Message reset();
	static Message status();
	static Message getConfig();
	static Message setConfig(const Message &from);
	static Message startScan();
	static Message askAdf();
};
