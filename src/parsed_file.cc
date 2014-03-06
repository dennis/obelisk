
#include "parsed_file.h"
#include "line.h"
#include "channel_line.h"
#include "channel.h"

void ParsedFile::addLine(std::string timestamp, std::string other) {
	lines.push_back(line_ptr(new Line(timestamp, other)));
}

void ParsedFile::addChannelLine(std::string timestamp, std::string channelId, std::string other) {
	lines.push_back(line_ptr(new ChannelLine(timestamp, channelId, other)));
	Channel& c = channels[channelId];
	if(c.empty()) {
		c.setName(channelId);
		c.setTimestamp(timestamp);
	}
	c.touch();
}

const ParsedFile::channels_t& ParsedFile::getChannels() {
	return channels;
}

const ParsedFile::lines_t& ParsedFile::getLines() {
	return lines;
}
