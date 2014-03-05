#ifndef __channel_line_h__
#define __channel_line_h__

#include <string>
#include <iostream>

#include "line.h"

class ChannelLine : public Line {
private:
	std::string channelId;

public:
	ChannelLine(const std::string, const std::string, const std::string);
	virtual bool matches(const Clause&);

	friend std::ostream& operator<<(std::ostream&, const ChannelLine&);
};

#endif
