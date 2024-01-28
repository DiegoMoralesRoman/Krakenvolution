#include <gtest/gtest.h>

#include "nodes/node.hpp"
#include "nodes/nodes.hpp"
#include "config/config.hpp"
#include "topics.hpp"

struct Context {
	int value;
};

INIT_NODE(test_node, Context);

void setup(core::topics::GlobalContext& global, Context& ctx) {

}

void end(core::topics::GlobalContext& global, Context& ctx) {

}


TEST(node, basic_node) {
}
