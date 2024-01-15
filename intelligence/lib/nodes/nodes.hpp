#pragma once

#include "nodes/node.hpp"
#include <memory>
#include <vector>

namespace core::nodes {
	using NodePtr = std::unique_ptr<core::nodes::Node>;
	std::vector<core::nodes::ApplicationNode> create_all_nodes(); 
}
