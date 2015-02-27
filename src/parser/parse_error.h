#ifndef __PARSE_ERROR_H__
#define __PARSE_ERROR_H__

#include <exception>

class ParseError : std::exception {
private:
	const std::string _error;
public:
	ParseError(const std::string error) : _error(error) {
	}

	virtual const char* what() const throw() {
		return _error.c_str();
	}
};

#endif
