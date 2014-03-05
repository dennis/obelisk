#ifndef __clause_h__
#define __clause_h__

#include <string>

class Clause {
private:
	std::string channelId;

public:
	void setChannelId(const std::string);
	const std::string& getChannelId() const;
	bool empty() const;
};

#endif
