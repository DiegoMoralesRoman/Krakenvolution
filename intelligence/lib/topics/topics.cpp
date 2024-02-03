#include "topics.hpp"
#include "easylogging/easylogging++.h"
#include <concepts>

template<typename T>
concept Serializable = requires (T t) {
	{ t.SerializeAsString() } -> std::same_as<std::string>;
};

template<Serializable T>
rxcpp::observable<std::string> map_to_serial(const rxcpp::observable<T>& obs) {
	return obs.map([](const T& value) { return value.SerializeAsString(); });
}

core::topics::Topics::Topics() {
	serialized.insert({"test", this->test.get_observable().map([](int value){ return std::to_string(value); })});
	// rxcpp::observable<Person> obs;
	// serialized.insert({"obs", map_to_serial()});
}
