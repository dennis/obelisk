#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include "obelisk.h"
#include "parser.h"

//static void showStrings (std::string name) { 
static void showStrings (std::pair<std::string, Channel> pair) { 
  std::cout 
  	  << " " << pair.second.getTimestamp() 
  	  << " [" << std::setfill(' ') << std::setw(2) << pair.second.getCount() << "]"
  	  << " " << pair.first
  	  << std::endl;
}

class OnlyShowMatchingLines {
private:
	const Clause clause;
public:
	OnlyShowMatchingLines(Clause clause_) : clause(clause_) {
	}

	void operator()(ParsedFile::line_ptr line) {
		if(line->matches(clause))
			std::cout << (*line) << std::endl;
	}
};

void Obelisk::process(std::istream& instream) {
	Parser parser;

	ParsedFile::parsed_file_ptr parsedFile = parser.parse(instream);

	if(clause.empty()) {
		ParsedFile::channels_t channels = parsedFile->getChannels();

		std::for_each(channels.begin(), channels.end(), showStrings);
	}
	else {
		OnlyShowMatchingLines foo(clause);
		ParsedFile::lines_t lines = parsedFile->getLines();

		std::for_each(lines.begin(), lines.end(), foo);
	}
}

void Obelisk::setClause(Clause clause_) {
	clause = clause_;
}
