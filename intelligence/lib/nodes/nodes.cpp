#include "nodes.hpp"

// Nodes
#include "nodes/lidar/lidar.hpp"

std::vector<nodes::NodePtr> nodes::create_all_nodes() {
	std::vector<NodePtr> nodes;

	// Add all nodes
	nodes.push_back(lidar::create_node());

	return nodes;
}
