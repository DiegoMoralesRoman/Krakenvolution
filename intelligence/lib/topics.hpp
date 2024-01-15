#pragma once

#include <atomic>
#include <rxcpp/rx.hpp>
// Messages
#include "easylogging/easylogging++.h"
#include "messages/detection.hpp"
#include "rxcpp/rx-observable.hpp"
#include "rxcpp/subjects/rx-subject.hpp"

namespace core {
	/*=============
	 System topics
	=============*/
	struct Topics {
		rxcpp::subjects::subject<messages::Detection> detections;
		rxcpp::subjects::subject<int> test;
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
	//
// Define custom nodes
#define INIT_NODE(name, context_class) \
	void setup(::core::GlobalContext& global, context_class& ctx); \
	void end(::core::GlobalContext& global, context_class& ctx); \
	::core::ApplicationNode create_node();
}
