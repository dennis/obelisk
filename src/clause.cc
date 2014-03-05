#include "clause.h"


void Clause::setChannelId(const std::string channelId_) {
	channelId = channelId_;
}

const std::string& Clause::getChannelId() const {
	return channelId;
}

bool Clause::empty() const {
	return channelId.size() == 0;
}
