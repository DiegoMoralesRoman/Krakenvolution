#include "nodes.hpp"

#include <vector>

auto core::nodes::create_all_nodes() -> std::vector<ApplicationNode> {
	std::vector<ApplicationNode> nodes;
	return nodes;
}

auto core::nodes::static_pool() -> std::vector<ApplicationNode>& {
	static std::vector<ApplicationNode> nodes;
	return nodes;
}
