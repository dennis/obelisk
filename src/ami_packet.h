#ifndef __AMI_PACKET_H__
#define __AMI_PACKET_H__

#include <map>
#include <string>
#include <utility>

class AmiPacket {
private:
	typedef std::multimap<std::string, std::string> kv_pairs_t;
	kv_pairs_t kv_pairs;
	std::string _source;
	std::string _destination;
	std::string _timestamp;
public:
	void set_peers(const std::string& source, const std::string& destination) {
		_source = source;
		_destination = destination;
	}

	void set_timestamp(const std::string& timestamp) {
		_timestamp = timestamp;
	}

	const std::string get_timestamp() const {
		return _timestamp;
	}

	const std::string get_source() const {
		return _source;
	}

	const std::string get_destination() const {
		return _destination;
	}

	void add_key_value(std::string key, std::string value) {
		kv_pairs.insert(std::pair<std::string, std::string>(key,value));
	}

	kv_pairs_t::size_type kv_size() const {
		return kv_pairs.size();
	}

	std::string get(const std::string key) const {
		auto it = kv_pairs.find(key);

		if(it != kv_pairs.end()) {
			return it->second;
		}
		else {
			return std::string();
		}
	}
};

#endif
