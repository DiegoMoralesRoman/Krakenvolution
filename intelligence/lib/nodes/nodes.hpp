#pragma once

#include "nodes/node.hpp"
#include <memory>
#include <vector>

namespace nodes {
	using NodePtr = std::unique_ptr<core::Node>;
	std::vector<NodePtr> create_all_nodes(); 
}
