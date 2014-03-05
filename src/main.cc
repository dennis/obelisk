#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "obelisk.h"

static void version() {
	std::cout << "obelisk v0.1" << std::endl;
}

static void usage() {
	version();
	std::cout << "usage: obelisk [-h] [-v] [-c channelid] [filename|-]" << std::endl << std::endl;
	std::cout << "  -h           : help (this output)" << std::endl;
	std::cout << "  -v           : shows version" << std::endl;
	std::cout << "  -c channelId : show lines matching channel" << std::endl;
}

struct noop {
    void operator()(...) const {}
};


int main(int argc, char* argv[]) {
	Clause clause;

	int c;
	while((c = getopt(argc, argv, "hvc:")) != -1) {
		switch(c) {
			case 'h':
				usage();
				return EXIT_SUCCESS;
				break;
			case 'v':
				version();
				return EXIT_SUCCESS;
				break;
			case 'c':

				clause.setChannelId(std::string(optarg));
				break;
		}
	}

	std::istream* input;
	
	if(optind < argc) {
		if(optind+1 == argc) {
			if(argv[optind][0] == '-' && argv[optind][1] == 0) {
				input = &std::cin;
			}
			else {
				input = new std::ifstream(argv[optind]);
			}
		}
		else {
			usage();
			return EXIT_FAILURE;
		}
	}
	else {
		input = &std::cin;
	}

	Obelisk obelisk;
	obelisk.setClause(clause);
	obelisk.process(*input);
}
