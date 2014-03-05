#ifndef __parsed_file_h__
#define __parsed_file_h__

#include <set>
#include <list>
#include <string>
#include <tr1/memory>

#include "line.h"

class ParsedFile {
public:
	typedef std::set<std::string> channels_t;
	typedef std::tr1::shared_ptr<Line> line_ptr;
	typedef std::list<line_ptr> lines_t;

private:
	lines_t lines;
	channels_t channels;

public:
	typedef std::tr1::shared_ptr<ParsedFile> parsed_file_ptr;

	void addLine(std::string, std::string);
	void addChannelLine(std::string, std::string, std::string);

	const channels_t& getChannels();
	const lines_t& getLines();
};
#endif
