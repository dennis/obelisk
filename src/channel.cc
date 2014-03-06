#include "channel.h"

Channel::Channel() : name(""), firstTimestamp(""), count(0) {
}

void Channel::setName(const std::string name_) {
	name = name_;
}

void Channel::setTimestamp(const std::string timestamp_) {
	firstTimestamp = timestamp_;
}

std::string Channel::getTimestamp() {
	return firstTimestamp;
}

int Channel::getCount() {
	return count;
}

void Channel::touch() {
	count++;
}

bool Channel::empty() {
	return count == 0;
}
