#ifndef __channel_h__
#define __channel_h__

#include <string>

class Channel {
private:
	std::string name;
	std::string firstTimestamp;
	int count;
public:
	Channel();
	void setName(const std::string);
	void setTimestamp(const std::string);
	std::string getTimestamp();
	int getCount();
	void touch();
	bool empty();
};
#endif
