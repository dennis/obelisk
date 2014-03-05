
#include "parsed_file.h"
#include "line.h"
#include "channel_line.h"

void ParsedFile::addLine(std::string timestamp, std::string other) {
	lines.push_back(line_ptr(new Line(timestamp, other)));
}

void ParsedFile::addChannelLine(std::string timestamp, std::string channelId, std::string other) {
	lines.push_back(line_ptr(new ChannelLine(timestamp, channelId, other)));
	channels.insert(channelId);
}

const ParsedFile::channels_t& ParsedFile::getChannels() {
	return channels;
}

const ParsedFile::lines_t& ParsedFile::getLines() {
	return lines;
}
