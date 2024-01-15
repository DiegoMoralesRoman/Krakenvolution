#include "nodes.hpp"

// Nodes
#include "nodes/lidar/lidar.hpp"
#include "nodes/node.hpp"

std::vector<core::ApplicationNode> nodes::create_all_nodes() {
	std::vector<core::ApplicationNode> nodes;

	// Add all nodes
	nodes.push_back(lidar::create_node());

	return nodes;
}
