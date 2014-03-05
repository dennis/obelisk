#include "line.h"

Line::Line(const std::string timestamp_, const std::string other_) : timestamp(timestamp_), other(other_) {
}

bool Line::matches(const Clause&) {
	return false;
}

std::ostream& operator<<(std::ostream& os, const Line& line) {
	os << line.timestamp << " " << line.other;
	return os;
}
