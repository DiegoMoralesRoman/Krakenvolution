#pragma once

#include <nodes/node.hpp>

#include <memory>
#include <vector>

namespace core::nodes {
	using NodePtr = std::unique_ptr<core::nodes::Node>;
	auto create_all_nodes() -> std::vector<core::nodes::ApplicationNode>; 
	auto static_pool() -> std::vector<ApplicationNode>&;
}
