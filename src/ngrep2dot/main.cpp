#include <fstream>
#include <iostream>
#include <map>
#include <memory>

#include "parser/parse_error.h"
#include "parser/ngrep_log_parser.h"

class DotGenerator {
private:
	const std::string DEFAULT_OPTIONS = ",color=yellow,style=filled";
	const std::string NEWCHANNEL_OPTIONS = ",color=green,style=filled";
	const std::string HANG_OPTIONS = ",color=red,style=filled";
	const std::string OTHER_EVENT_OPTIONS = ",color=grey";
	const std::string USER_EVENT_OPTIONS = ",color=cyan,style=filled";

	std::list<std::string> timestamps;
	std::map<std::string, std::string> channel_position;

	void output_node(const AmiPacket* pkt, const std::string& node_name, const std::string& label, const std::string& options = "") {
		std::string opt;

		if(options.size() == 0) 
			opt = DEFAULT_OPTIONS;
		else
			opt = options;
		std::cout << "{ " << std::endl;
		std::cout << "  rank = same; " << std::endl;
		std::cout << "  \"" << pkt->get_timestamp() << "\"[fontsize=16, shape=plaintext]; " << std::endl;
		std::cout << "  " << node_name << "[label=\"" << node_name << ": " << label << "\"" << opt << "];" << std::endl;
		std::cout << "} " << std::endl;
		timestamps.push_back(pkt->get_timestamp());
	}

	void output_edge(const std::string& from, const std::string& to, const std::string& label, const std::string&options = "") {
		std::cout << from << " -> " << to << "[label=\"" << label << "\"" << options << "];" << std::endl;
	}

	const std::string get_channel_position(const std::string& node_name, const std::string& channel) {
		auto channel_it = channel_position.find(channel);

		if(channel_it == channel_position.end()) {
			std::cout << " // " << node_name << " Warning - unknown channel '" << channel << "' - creating" << std::endl;
			channel_position[channel] = node_name;
		}

		return channel_position[channel];
	}

public:
	void handleNewChannel(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& channel = pkt->get("Channel");

		output_node(pkt, node_name, "Newchannel\\nChannel:" + channel, NEWCHANNEL_OPTIONS);

		auto it = channel_position.find(channel);
		if(it != channel_position.end()) {
			std::cout << " // Warning: Channel already exists - overwriting with new channel" << std::endl;
		}

		std::cout << " // setting " << channel << " to " << node_name << std::endl;
		channel_position[channel] = node_name;

		// if channelname ends with ;2 - then link it to ;1 if found
		auto it2 = channel.rbegin();
		if(*it2 == '2' && ++it2 != channel.rend() && *it2 == ';') {
			const std::string& other_channel = channel.substr(0, channel.size()-1) + '1';
			auto it3 = channel_position.find(other_channel);
			if(it3 != channel_position.end()) {
				std::cout << " // Linked " << channel << " to " << it3->second << " based by name" << std::endl;
				output_edge(it3->second, node_name, other_channel, ",style=dotted");
			}
		}
	}

	void handleNewState(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& channel = pkt->get("Channel");

		output_node(pkt, node_name, "Newstate\\n" + pkt->get("ChannelStateDesc"));

		auto it = channel_position.find(channel);
		if(it == channel_position.end()) {
			std::cout << " // Warning: Channel '" << channel << "' not found" << std::endl;
		}
		else {
			const std::string& origin = channel_position[channel];
			output_edge(origin, node_name, channel);
		}
		channel_position[channel] = node_name;
	}

	void handleNewCallerId(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& channel = pkt->get("Channel");

		output_node(pkt, node_name, "NewCallerid\\n" + pkt->get("CallerIDNum") + "\\n" + pkt->get("CallerIDName"));

		auto it = channel_position.find(channel);
		if(it == channel_position.end()) {
			std::cout << " // Warning: Channel '" << channel << "' not found" << std::endl;
		}
		else {
			const std::string& origin = channel_position[channel];
			output_edge(origin, node_name, channel);
		}

		channel_position[channel] = node_name;
	}

	void handleNewAccountCode(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& channel = pkt->get("Channel");

		output_node(pkt, node_name, "NewAccountCode");

		auto it = channel_position.find(channel);
		if(it == channel_position.end()) {
			std::cout << " // Warning: Channel '" << channel << "' not found" << std::endl;
		}
		else {
			const std::string& origin = channel_position[channel];
			output_edge(origin, node_name, channel);
		}
		channel_position[channel] = node_name;
	}

	void handleHangup(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& channel = pkt->get("Channel");

		output_node(pkt, node_name, "Hangup\\n" + pkt->get("Cause-txt"), HANG_OPTIONS);

		auto it = channel_position.find(channel);
		if(it == channel_position.end()) {
			std::cout << " // Warning: Channel '" << channel << "' not found" << std::endl;
		}
		else {
			const std::string& origin = channel_position[channel];

			output_edge(origin, node_name, channel);
		}
		channel_position[channel] = node_name;
	}

	void handleDial(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& sub_event = pkt->get("SubEvent");
		const std::string& channel = pkt->get("Channel");
		const std::string& channel_origin = get_channel_position(node_name, channel);

		if(sub_event.compare("Begin") == 0) {
			const std::string& destination = pkt->get("Destination");

			const std::string& destination_origin = get_channel_position(node_name, destination);

			output_node(pkt, node_name, "Dial/Begin\\nDialstring: " + pkt->get("Dialstring"));
			output_edge(channel_origin, node_name, channel);
			output_edge(destination_origin, node_name, destination);

			channel_position[channel] = node_name;
			channel_position[destination] = node_name;
		}
		else if(sub_event.compare("End") == 0) {
			output_node(pkt, node_name, "Dial/End\\n" + pkt->get("DialStatus"));
			output_edge(channel_origin, node_name, channel);

			channel_position[channel] = node_name;
		}
		else {
			std::cout << " // Warning: Unknown subevent for dial: " << sub_event << std::endl;
		}
	}

	void handleBridge(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& channel1 = pkt->get("Channel1");
		const std::string& channel2 = pkt->get("Channel2");

		const std::string& channel1_origin = get_channel_position(node_name, channel1);
		const std::string& channel2_origin = get_channel_position(node_name, channel2);

		output_node(pkt, node_name, "Bridge");
		output_edge(channel1_origin, node_name, channel1);
		output_edge(channel2_origin, node_name, channel2);

		channel_position[channel1] = node_name;
		channel_position[channel2] = node_name;
	}

	void handleUnlink(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& channel1 = pkt->get("Channel1");
		const std::string& channel2 = pkt->get("Channel2");

		const std::string& channel1_origin = get_channel_position(node_name, channel1);
		const std::string& channel2_origin = get_channel_position(node_name, channel2);

		output_node(pkt, node_name, "Unlink");
		output_edge(channel1_origin, node_name, channel1);
		output_edge(channel2_origin, node_name, channel2);

		channel_position[channel1] = node_name;
		channel_position[channel2] = node_name;
	}

	void handleMasquerade(const std::string& node_name, const AmiPacket *pkt) {
		const std::string& clone = pkt->get("Clone");
		const std::string& original = pkt->get("Original");

		const std::string& original_origin = get_channel_position(node_name, original);

		output_node(pkt, node_name, "Masquerade clone: " + clone);
		output_edge(original_origin, node_name, original);

		auto it = channel_position.find(clone);
		if(it != channel_position.end()) {
			std::cout << " // Warning: Channel '" << clone << " 'already exists - overwriting with new channel" << std::endl;
		}

		std::cout << " // setting " << clone << " to " << node_name << std::endl;
		channel_position[clone] = node_name;
		channel_position[original] = node_name;
	}

	void handleRename(const std::string& node_name, const AmiPacket* pkt) {
		const std::string& channel = pkt->get("Channel");
		const std::string& new_name = pkt->get("Newname");

		const std::string& channel_origin = get_channel_position(node_name, channel);

		output_node(pkt, node_name, "Rename\\n" + new_name);
		output_edge(channel_origin, node_name, channel);

		auto it = channel_position.find(channel);
		if(it != channel_position.end()) {
			channel_position.erase(it);
			std::cout << " // Deleted channel: " << channel << std::endl;
		}

		channel_position[new_name] = node_name;
	}

	void handleQueueMemberStatus(const std::string& node_name, const AmiPacket *pkt) {
		output_node(pkt, node_name, "QueueMemberStatus\\n" + pkt->get("Queue") + " " + pkt->get("MemberName"), OTHER_EVENT_OPTIONS);
	}

	void handleQueueParams(const std::string& node_name, const AmiPacket *pkt) {
		output_node(pkt, node_name, "QueueParams", OTHER_EVENT_OPTIONS);
	}

	void handleQueueMember(const std::string& node_name, const AmiPacket *pkt) {
		output_node(pkt, node_name, "QueueMember", OTHER_EVENT_OPTIONS);
	}

	void handleQueueStatusComplete(const std::string& node_name, const AmiPacket *pkt) {
		output_node(pkt, node_name, "QueueStatusComplete", OTHER_EVENT_OPTIONS);
	}

	void handleExtensionStatus(const std::string& node_name, const AmiPacket *pkt) {
		output_node(pkt, node_name, "ExtensionStatus\\n" + pkt->get("Exten"), OTHER_EVENT_OPTIONS);
	}

	void handleUserEvent(const std::string& node_name, const AmiPacket *pkt) {
		output_node(pkt, node_name, "UserEvent " + pkt->get("UserEvent"), USER_EVENT_OPTIONS);
	}

	void handleUnknown(const std::string& node_name, const AmiPacket *pkt) {
		std::cout << "// Unknown packet: " << node_name << ": " << pkt->get("Event") << std::endl;
	}

	void prefix() {
		std::cout << "digraph { " << std::endl;
	}

	void postfix() {
		std::cout << " // timeline" << std::endl;
		std::cout << "{ " << std::endl;
		std::cout << "node [shape=plaintext, fontsize=16];" << std::endl;

		std::string last_timestamp;
		for(auto it = timestamps.begin(); it != timestamps.end(); ++it) {
			const std::string timestamp = *it;

			if(last_timestamp.compare(timestamp) != 0) {
				if(it != timestamps.begin())
					std::cout << " -> ";
				std::cout << "\"" << timestamp << "\" " << std::endl;
			}
			
			last_timestamp = timestamp;
		}
		std::cout << ";" << std::endl << "} " << std::endl;
		std::cout << "}" << std::endl;
	}
};

class Analyser : public NgrepLogParser::Handler {
private:
	int pkt_count = 0;
	DotGenerator dot;
public:
	virtual void newAmiPacket(const AmiPacket* pkt) {
		pkt_count++;
		const std::string& event = pkt->get("Event");
		const std::string node_name = "pkt" + std::to_string(pkt_count);

		if(event.compare("Newchannel")==0)
			dot.handleNewChannel(node_name, pkt);
		else if(event.compare("Newstate") == 0)
			dot.handleNewState(node_name, pkt);
		else if(event.compare("NewCallerid") == 0)
			dot.handleNewCallerId(node_name, pkt);
		else if(event.compare("NewAccountCode") == 0)
			dot.handleNewAccountCode(node_name, pkt);
		else if(event.compare("Hangup") == 0)
			dot.handleHangup(node_name, pkt);
		else if(event.compare("Dial")==0) 
			dot.handleDial(node_name, pkt);
		else if(event.compare("Bridge")==0)
			dot.handleBridge(node_name, pkt);
		else if(event.compare("Unlink")==0)
			dot.handleUnlink(node_name, pkt);
		else if(event.compare("Masquerade")==0)
			dot.handleMasquerade(node_name, pkt);
		else if(event.compare("Rename")==0)
			dot.handleRename(node_name, pkt);
		else if(event.compare("QueueMemberStatus")==0)
			dot.handleQueueMemberStatus(node_name, pkt);
		else if(event.compare("QueueParams")==0)
			dot.handleQueueParams(node_name, pkt);
		else if(event.compare("QueueMember")==0)
			dot.handleQueueMember(node_name, pkt);
		else if(event.compare("QueueStatusComplete")==0)
			dot.handleQueueStatusComplete(node_name, pkt);
		else if(event.compare("ExtensionStatus")==0)
			dot.handleExtensionStatus(node_name, pkt);
		else if(event.compare("UserEvent")==0)
			dot.handleUserEvent(node_name, pkt);
		else
			dot.handleUnknown(node_name, pkt);
	}

	void postfix() {
		dot.postfix();
	}

	void prefix() {
		dot.prefix();
	}
};

int main(int argc, char* argv[]) {
	if(argc == 2) {
		std::ifstream infile(argv[1]);

		NgrepLogParser parser = NgrepLogParser(infile);
		Analyser analyzer = Analyser();
		try {
			analyzer.prefix();
			parser.parse(analyzer);
			analyzer.postfix();
		}
		catch(ParseError* error) {
			std::cerr << error->what() << std::endl;
		}
	}
	else {
		std::cerr << "usage: " << argv[0] << " <filename>" << std::endl;
	}
}
