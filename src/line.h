#ifndef __line_h__
#define __line_h__

#include <string>
#include <iostream>

#include "clause.h"

class Line {
protected:
	std::string timestamp;
	std::string other;
public:
	Line(const std::string, const std::string);
	virtual bool matches(const Clause&);

	friend std::ostream& operator<<(std::ostream&, const Line&);
};
#endif
