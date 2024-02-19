#pragma once

#include <string>
#include <rxcpp/rx.hpp>
#include <concepts>
#include <utility>
#include "easylogging/easylogging++.h"
#include "nodes/nodes.hpp"
#include "channel.hpp"
#include "rxcpp/subjects/rx-subject.hpp"

namespace core::serial{
	template<typename T>
	concept ProtoMessage = requires (T t) {
		{ t.SerializeAsString() } -> std::same_as<std::string>;
		{ t.ParseFromString(std::declval<std::string>()) } -> std::same_as<bool>;
	};


	template <ProtoMessage Message> 
	auto gen_mapping(rxcpp::subjects::subject<Message> subject) -> ObserverMapping {
		auto from_source = rxcpp::subjects::subject<std::string>();
		ObserverMapping mapping {
			.from_source = from_source,
			.to_core_channel = Channel {
				.rx = subject.get_observable().map([](const Message& msg) { return msg.SerializeAsString(); }),
				.tx = from_source.get_subscriber(),
				.UID = static_cast<size_t>(random()),
			}
		};
		
		from_source.get_observable().subscribe([subject](const std::string& str) {
			Message new_message;
			if (new_message.ParseFromString(str)) {
				subject.get_subscriber().on_next(new_message);
			} else {
				LOG(ERROR) << "Failed to deserialize message of type " << typeid(Message).name();
			}
		});

		return mapping;
	}

	auto mapping(nodes::GlobalContext& ctx) -> std::unordered_map<std::string, ObserverMapping>;
	// decltype(auto) mapping(nodes::GlobalContext& ctx);
}
