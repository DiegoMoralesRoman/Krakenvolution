#include <gtest/gtest.h>

#include "nodes/node.hpp"
#include "nodes/nodes.hpp"
#include "config/config.hpp"
#include "topics/topics.hpp"

struct Context {
	int value;
};

INIT_NODE(test_node, Context);

void setup(core::topics::GlobalContext& global, Context& ctx, const core::config::Config& cfg) {

}

void end(core::topics::GlobalContext& global, Context& ctx, const core::config::Config& cfg) {

}


TEST(node, basic_node) {
}
