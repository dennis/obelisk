#ifndef __obelisk_h__
#define __obelisk_h__

#include <iostream>

#include "clause.h"

class Obelisk {
private:
	Clause clause;
public:
	void process(std::istream& in);
	void setClause(Clause);
};

#endif
