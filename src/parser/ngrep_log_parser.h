#ifndef __NGREP_LOG_PARSER_H__
#define __NGREP_LOG_PARSER_H__

#include <map>
#include <list>

#include "../ami_packet.h"

class NgrepLogParser {
public:
	typedef std::list<AmiPacket> complete_packets_t;

	class Handler {
	public:
		virtual void newAmiPacket(const AmiPacket*) = 0;
	};
private:
	std::istream& _input;
	std::string _line;
	std::string _current_participants; // IP:PORT->IP:PORT
	std::string _source;
	std::string _destination;
	std::string _timestamp;
	std::map<std::string, AmiPacket> packets;

	complete_packets_t _complete_packets;
	int _lineno;

	bool next();
	bool find_empty_line();
	bool parse_context();
	void end_of_packet(NgrepLogParser::Handler&);
	bool is_context_line();
	bool parse_packet();
	bool add_key_value_pairs();

public:
	NgrepLogParser(std::istream& input);

	complete_packets_t::size_type parsed_packets_count();
	void parse(NgrepLogParser::Handler&);
};

#endif
