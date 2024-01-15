#include "nodes.hpp"

// Nodes
#include "nodes/lidar/lidar.hpp"
#include "nodes/node.hpp"

std::vector<core::nodes::ApplicationNode> core::nodes::create_all_nodes() {
	std::vector<ApplicationNode> nodes;

	// Add all nodes
	nodes.push_back(nodes::lidar::create_node());

	return nodes;
}
