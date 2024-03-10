#pragma once
#include <serial/channel.hpp>

#include <easylogging/easylogging++.h>
#include <rxcpp/subjects/rx-subject.hpp>
#include <google/protobuf/descriptor.h>

#include <topics/topics.hpp>
#include "topics.hpp"

#include <string>
#include <concepts>
#include <utility>

namespace user::serial{
	template<typename T>
	concept ProtoMessage = requires (T t) {
		{ t.SerializeAsString() } -> std::same_as<std::string>;
		{ t.ParseFromString(std::declval<std::string>()) } -> std::same_as<bool>;
		{ T::descriptor() } -> std::same_as<const google::protobuf::Descriptor*>;
		// { T::descriptor().name() } -> std::same_as<const std::string&>;
	};


	template <ProtoMessage Message> 
	auto gen_mapping(rxcpp::subjects::subject<Message> subject) -> core::serial::ObserverMapping {
		auto from_source = rxcpp::subjects::subject<std::string>();
		core::serial::ObserverMapping mapping {
			.from_source = from_source,
			.to_core_channel = core::serial::Channel<> {
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

	auto mapping(core::topics::GlobalContext& ctx) -> std::unordered_map<std::string, core::serial::ObserverMapping>;
}
