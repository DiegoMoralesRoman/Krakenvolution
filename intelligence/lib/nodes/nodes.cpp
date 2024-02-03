#include "nodes.hpp"

// Nodes
#include "nodes/robot/lidar/lidar.hpp"
#include "nodes/extensions/remote/remote.hpp"
#include "nodes/node.hpp"

std::vector<core::nodes::ApplicationNode> core::nodes::create_all_nodes() {
	std::vector<ApplicationNode> nodes;

	// Add all nodes
	nodes.push_back(nodes::lidar::create_node());

	// Add all extensions
	nodes.push_back(extensions::remote::create_node());

	return nodes;
}
