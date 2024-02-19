#pragma once

#include <atomic>
#include <rxcpp/rx.hpp>
#include <unordered_map>

// Messages
#include "rxcpp/rx-observable.hpp"
#include "rxcpp/subjects/rx-subject.hpp"
// Protobuf messages
#include "messages/test.pb.h"
#include "serial/channel.hpp"

namespace core::topics {
	/*=============
	 System topics
	=============*/
	struct Topics {
		Topics();
		// rxcpp::subjects::subject<messages::Detection> detections;
		rxcpp::subjects::subject<Person> person;
		std::unordered_map<std::string, serial::ObserverMapping> serialized;
	};

	struct GlobalContext {
		Topics topics;
		const std::atomic<bool>& running;
		rxcpp::subjects::subject<bool> stop_subject;
		rxcpp::observable<bool> stop_signal;

		GlobalContext(const std::atomic<bool>* running) 
			: running(*running), stop_signal(stop_subject.get_observable()) {
			// Start a separate thread for the running.wait logic
			std::thread([this]() {
				this->running.wait(true);
				stop_subject.get_subscriber().on_next(static_cast<bool>(this->running));
				stop_subject.get_subscriber().on_completed();
			}).detach(); // Detach the thread to run independently
		}
	};
// Define custom nodes
#define INIT_NODE(name, context_class) \
	auto setup(::core::topics::GlobalContext& global, context_class& ctx, const ::core::config::Config& cfg) -> void; \
	auto end(::core::topics::GlobalContext& global, context_class& ctx, const ::core::config::Config& cfg) -> void; \
	auto create_node() -> ::core::nodes::ApplicationNode;
}
