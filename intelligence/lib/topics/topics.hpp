#pragma once

#include <atomic>
#include <rxcpp/rx.hpp>
#include <unordered_map>

#include <topics.hpp>

// Messages
#include "rxcpp/rx-observable.hpp"
#include "rxcpp/subjects/rx-subject.hpp"
//
// Protobuf messages
#include "messages/test.pb.h"

namespace core::topics {
	/*=============
	 System topics
	=============*/

	struct GlobalContext {
		user::topics::Topics topics;
		std::atomic<bool>& running;
		rxcpp::subjects::subject<bool> stop_subject;
		rxcpp::observable<bool> stop_signal;

		GlobalContext(std::atomic<bool>& running) 
			: running(running), stop_signal(stop_subject.get_observable()) {
			// Start a separate thread for the running.wait logic
			std::thread([this]() {
				this->running.wait(true);
				stop_subject.get_subscriber().on_next(static_cast<bool>(this->running));
				stop_subject.get_subscriber().on_completed();
			}).detach(); // Detach the thread to run independently
		}
	};
}
