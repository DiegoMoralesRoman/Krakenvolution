#include "topics.hpp"

std::unordered_map<std::string, core::serial::ObserverMapping> core::serial::mapping(nodes::GlobalContext &ctx) {
	std::unordered_map<std::string, ObserverMapping> map;
	topics::Topics& topics = ctx.topics;

	map.insert({"person", gen_mapping(topics.person)});

	return map;
}
