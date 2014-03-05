#include "channel_line.h"

ChannelLine::ChannelLine(const std::string timestamp_, const std::string channelId_, const std::string other_) :
	Line(timestamp_, other_), channelId(channelId_) {
}

bool ChannelLine::matches(const Clause& clause) {
	if(clause.empty())
		return true;

	return clause.getChannelId().compare(channelId) == 0;
}

std::ostream& operator<<(std::ostream& os, const ChannelLine& line) {
	os << line.timestamp << " " << line.channelId << " " << line.other;
	return os;
}
