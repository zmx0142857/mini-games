#ifndef STR_H
#define STR_H

// TODO: windows 下的乱码问题
// chcp 65001
// g++ test.cpp -o test -Wall -fexec-charset=utf-8

#include <iostream>
#include <sstream>
#include <vector>

/* utf-8 的 6 种形态:
 * 0xxxxxxx
 * 110xxxxx 10xxxxxx
 * 1110xxxx 10xxxxxx 10xxxxxx
 * 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
 * 我们只考虑前 3 种
 */

class Char {

public:
	unsigned short ord;

	explicit Char(unsigned short d=0): ord(d) {}

	explicit Char(const std::string &str) {
		std::istringstream is(str);
		decode(is);
	}

	explicit Char(std::istream &is) {
		decode(is);
	}

	operator bool() const {
		return ord;
	}

	bool is_ascii() const {
		return ord < 0x80;
	}

	// return count of bytes when converted to utf-8
	size_t size() const {
		if (ord < 0x80)
			return 1;
		else if (ord < 0x800)
			return 2;
		else // if (ord < 0x10000)
			return 3;
		// 只考虑前 3 种 utf-8 类型 (最多 3 字节)
	}

	// encode with utf-8
	std::string to_string() const {
		size_t sz = size();
		if (sz == 1)
			return std::string({char(ord)});
		else if (sz == 2) {
			char c0 = (ord & 0x3f) | 0x80;
			char c1 = ((ord >> 6) & 0x1f) | 0xc0;
			return std::string({c1, c0});
		} else { // if (sz == 3)
			char c0 = (ord & 0x3f) | 0x80;
			char c1 = ((ord >> 6) & 0x3f) | 0x80;
			char c2 = ((ord >> 12) & 0xf) | 0xe0;
			return std::string({c2, c1, c0});
		}
	}

	friend std::ostream &operator<<(std::ostream &os, const Char &c) {
		return os << c.to_string();
	}

	friend std::istream &operator>>(std::istream &is, Char &c) {
		c.decode(is);
		return is;
	}

	bool operator==(const Char &c) {
		return this->ord == c.ord;
	}

	bool operator!=(const Char &c) {
		return !(*this == c);
	}

	bool operator==(unsigned short num) {
		return this->ord == num;
	}

	bool operator!=(unsigned short num) {
		return !(*this == num);
	}

	static void test() {
		using namespace std;
		string str;
		// 20013: 中
		// 25991: 文
		// 26834: 棒
		while (cin >> str) {
			Char c(str);
			cout << c.ord << ": " << Char(c.ord) << endl;
		}
	}

private:
	// take first Char only
	void decode(std::istream &is) {
		ord = 0;
		if (!is) bad_encoding(is, 0);
		char c;
		is.get(c);
		if (!is) bad_encoding(is, c);
		//std::cout << "first: " << int((unsigned char)c) << std::endl;
		if ((c & 0x80) == 0) { // size == 1
			ord = c;
		} else if ((c & 0xe0) == 0xc0) { // size == 2
			ord = c & 0x1f;
			read_next(is);
		} else { // size == 3
			if ((c & 0xf0) != 0xe0) {
				bad_encoding(is, c);
			}
			ord = c & 0xf;
			read_next(is);
			read_next(is);
		}
	}

	void read_next(std::istream &is) {
		char c;
		ord <<= 6;
		is.get(c);
		if (!is) bad_encoding(is, c);
		//std::cout << "next: " << int((unsigned char)c) << std::endl;
		if ((c & 0xc0) != 0x80) {
			bad_encoding(is, c);
		}
		ord |= (c & 0x3f);
	}

	void bad_encoding(std::istream &is, int c) {
		if (c == 0)
			is.setstate(std::ios::eofbit);
		else
			is.setstate(std::ios::badbit);
		//std::cout << "decode failed\n";
		/*
		std::ostringstream os;
		os << "cannot decode byte: 0x" << std::hex << c;
		throw std::invalid_argument(os.str());
		*/
	}

};

class String {

	std::vector<Char> data;

public:
	String() {} 

	String(const std::string &str) {
		std::istringstream is(str);
		decode(is);
	}

	// encode with utf-8
	std::string to_string() const {
		std::ostringstream os;
		for (const auto &c : data) {
			os << c;
		}
		return os.str();
	}

	friend std::ostream &operator<<(std::ostream &os, const String &s) {
		return os << s.to_string();
	}

	size_t size() const {
		return data.size();
	}

	Char &operator[](size_t index) {
		return data[index];
	}

	const Char &operator[](size_t index) const {
		return data[index];
	}

	static void test() {
		using namespace std;
		string str;
		while (cin >> str) {
			String s(str);
			cout << s << endl;
			for (const auto &c : s.data) {
				cout << c.ord << ' ';
			}
			cout << endl;
		}
	}

private:
	void decode(std::istream &is) {
		if (!is) return;
		while (true) {
			Char c(is);
			if (!is) return;
			data.push_back(c);
		}
	}

};

// 标准库的 getline 忽略空行, 而这个不会
std::istream &getline(std::istream &is, String &str) {
	std::string line;
	char c;
	while (is.get(c) && c != '\n') {
		line += c;
	}
	str = String(line);
	return is;
}

#endif // STR_H
