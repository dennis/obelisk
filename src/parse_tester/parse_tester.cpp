#include <fstream>
#include <iostream>

#include "ami_packet.h"
#include "parser/parse_error.h"
#include "parser/ngrep_log_parser.h"

class PacketCounter : public NgrepLogParser::Handler {
private:
	int counter = 0;
public:
	virtual void newAmiPacket(const AmiPacket*) {
		counter++;
	}

	int getCount() {
		return counter;
	}
};

int main(int argc, char* argv[]) {
	if(argc == 2) {
		std::ifstream infile(argv[1]);

		NgrepLogParser parser = NgrepLogParser(infile);
		PacketCounter counter = PacketCounter();
		try {
			parser.parse(counter);
			std::cout <<  counter.getCount() << std::endl;
		}
		catch(ParseError* error) {
			std::cerr << error->what() << std::endl;
		}
	}
	else {
		std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
	}
}
