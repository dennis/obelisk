#ifndef __parser_h__
#define __parser_h__

#include <sys/types.h>
#include <regex.h>

#include <sstream>
#include <string>

#include "parsed_file.h"

class Parser {
private:
	regex_t preg;
	bool pregInitialized;
	void parseLine(std::string&, ParsedFile::parsed_file_ptr);

public:
	Parser();
	ParsedFile::parsed_file_ptr parse(std::istream&);
};

#endif

