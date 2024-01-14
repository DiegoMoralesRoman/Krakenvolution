#pragma once

#include <rxcpp/rx.hpp>

// Messages
#include "messages/detection.hpp"

namespace core {
	struct Topics {
		rxcpp::subjects::subject<messages::Detection> detections;
	};

	struct GlobalContext {
		Topics topics;
	};
	//
// Define custom nodes
#define INIT_NODE(name, context_class) \
	void setup(::core::GlobalContext& global, context_class& ctx); \
	void end(::core::GlobalContext& global, context_class& ctx); \
	::nodes::NodePtr create_node();

#define INIT_NODE_WITH_LOOP(name, context_class) \
	void setup(::core::GlobalContext& global, context_class& ctx); \
	void end(::core::GlobalContext& global, context_class& ctx); \
	void loop(::core::GlobalContext& global, context_class& ctx); \
	::nodes::NodePtr create_node();
}
