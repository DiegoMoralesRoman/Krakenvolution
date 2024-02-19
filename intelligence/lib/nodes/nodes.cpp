#include "nodes.hpp"

// Nodes
#include "nodes/robot/lidar/lidar.hpp"
#include "nodes/extensions/remote/remote.hpp"
#include "nodes/node.hpp"
#include "nodes/robot/test/test.hpp"

auto core::nodes::create_all_nodes() -> std::vector<core::nodes::ApplicationNode> {
	std::vector<ApplicationNode> nodes;

	// Add all nodes
	nodes.push_back(nodes::lidar::create_node());
	nodes.push_back(nodes::test::create_node());

	// Add all extensions
	nodes.push_back(extensions::remote::create_node());

	return nodes;
}
