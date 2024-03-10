#include "serial_topics.hpp"

#define MAP_TOPIC(name) \
	map.insert({#name, gen_mapping(topics.name)})

auto user::serial::mapping(core::topics::GlobalContext &ctx) -> std::unordered_map<std::string, core::serial::ObserverMapping> {
	std::unordered_map<std::string, core::serial::ObserverMapping> map;
	topics::Topics& topics = ctx.topics;

	MAP_TOPIC(person);

	return map;
}

#undef MAP_TOPIC
