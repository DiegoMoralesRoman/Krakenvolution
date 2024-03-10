#include <gtest/gtest.h>

#include <nodes/node_definitions.hpp>

#include "config/config.hpp"
#include "topics/topics.hpp"

#include <easylogging/easylogging++.h>

INITIALIZE_EASYLOGGINGPP


namespace core::nodes::test_node {
	struct Context {
		int value;
	};

	INIT_NODE(test_node, Context, nodes);
}

void core::nodes::test_node::setup(core::topics::GlobalContext& global, core::nodes::test_node::Context& ctx, const core::config::Config& cfg) {

}

void core::nodes::test_node::end(core::topics::GlobalContext& global, core::nodes::test_node::Context& ctx, const core::config::Config& cfg) {

}


TEST(node, basic_node) {
}
