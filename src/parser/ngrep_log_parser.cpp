#include <algorithm>
#include <sstream>

#include "parse_error.h"
#include "ngrep_log_parser.h"

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

NgrepLogParser::NgrepLogParser(std::istream& input) : _input(input), _lineno(0) {
}

bool NgrepLogParser::next() {
	_lineno++;
	std::getline(_input, _line);

	if(_input.bad())
		throw new ParseError("i/o error while parsing");


	if(_input.eof()) {
		return false;
	}
	else {
		if(*_line.rbegin() == '.')
			_line.pop_back();
		return true;
	}
}

bool NgrepLogParser::find_empty_line() {
	if(!next())
		return false;

	while (!_line.empty()) {
		if(!next())
			return false;
	}

	return true;
}

bool NgrepLogParser::parse_context() {
	// T 2015/02/13 13:03:28.211354 10.0.1.5:5038 -> 10.0.1.5:54877 [AP]
	std::string parsed;
	std::string date;
	std::string time;

	std::stringstream is(_line);
	int c = 0;

	while(std::getline(is,parsed,' ')) {
		
		switch(c) {
			case 0: // "T"
				if(parsed.compare("T") != 0)
					throw new ParseError(std::string("Error parsing context line at line #" + std::to_string(_lineno)));
				break;
			case 1: // "2015/02/13"
				date = parsed;
				break;
			case 2: // "13:03:28.211354"
				time = parsed;

				_timestamp = time;
				break;
			case 3: // "10.0.1.5:5038"
				_source = parsed;
				break;
			case 4: // "->"
				break;
			case 5: // "10.0.1.5:54877
				_destination = parsed;
				break;
			case 6: // "[AP]"
				// Ignore
				break;
			default:
				throw new ParseError(std::string("Error parsing context line at line #" + std::to_string(_lineno)));
		}

		c++;
	}

	_current_participants = _source + "->" + _destination;

	return true;
}

void NgrepLogParser::end_of_packet(NgrepLogParser::Handler& handler) {
	//_complete_packets.push_back(packets[_current_participants]);
	packets[_current_participants].set_peers(_source, _destination);
	packets[_current_participants].set_timestamp(_timestamp);
	handler.newAmiPacket(&packets[_current_participants]);
	packets.erase(_current_participants);
}

bool NgrepLogParser::is_context_line() {
	return *_line.begin() == 'T' && *_line.rbegin() == ']';
}

bool NgrepLogParser::parse_packet() {
	while(!_line.empty()) {
		add_key_value_pairs();

		if(!next())
			return false;
	}

	return true;
}

NgrepLogParser::complete_packets_t::size_type  NgrepLogParser::parsed_packets_count() {
	return _complete_packets.size();
}

void NgrepLogParser::parse(NgrepLogParser::Handler& handler) {
	// skip header from ngrep
	if(!find_empty_line())
		return;

	if(!next())
		return;

	// main parsing
	while(!_line.empty()) {
		if(is_context_line()) {
			if(!parse_context())
				break;
			
			if(!next())
				break;
		}
		
		do {
			if(!parse_packet())
				break;
			if(!next())
				break;
			if(!_line.empty()) 
				end_of_packet(handler);
		}while(!_line.empty());

		if(!next())
			break;

		if(_line.empty()) {
			if(!next()) {
				break;
			}

			if(!_line.empty())
				end_of_packet(handler);
		}
	}
	if(_input.eof()) {
		end_of_packet(handler);
	}
}


bool NgrepLogParser::add_key_value_pairs() {
	const std::string delimiter = ":";

	std::string::size_type pos = _line.find(delimiter);
	std::string key = _line.substr(0, pos);
	std::string value = _line.substr(pos+delimiter.size());
	if(*value.rbegin() == '.')
		value.pop_back();

	trim(key);
	trim(value);

	packets[_current_participants].add_key_value(key, value);

	return true;
}

