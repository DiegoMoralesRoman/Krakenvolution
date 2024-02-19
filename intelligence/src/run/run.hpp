#pragma once

#include <atomic>
#include <memory>
#include "nodes/node.hpp"
#include "parser/parser.hpp"
#include <optional>
#include <thread>
#include <vector>
#include "topics/topics.hpp"

namespace run {
	// Parsing
	auto config_easylogging(int argc, char* argv[]) -> void;
	auto  parse_cli(int argc, char* argv[]) -> std::unique_ptr<parser::Options>;

	// Nodes
	auto init_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, core::config::Config& cfg) -> void;
	auto teardown_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, core::config::Config& cfg) -> void;

	// Graphics
	auto init_graphics_thread(std::atomic<bool>& running, core::topics::GlobalContext& global) -> std::optional<std::thread>;

	// Cleanup
	auto shutdown_application() -> void;

	// Options
	auto config_from_path(const std::string& path) -> core::config::Config;
}
