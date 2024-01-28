#pragma once

#include <atomic>
#include <memory>
#include "nodes/node.hpp"
#include "parser/parser.hpp"
#include <optional>
#include <thread>
#include <vector>
#include "topics.hpp"

namespace run {
	// Parsing
	void config_easylogging(int argc, char* argv[]);
	std::unique_ptr<parser::Options> parse_cli(int argc, char* argv[]);

	// Nodes
	void init_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, const core::config::Config& cfg);
	void teardown_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, const core::config::Config& cfg);

	// Graphics
	std::optional<std::thread> init_graphics_thread(std::atomic<bool>& running, core::topics::GlobalContext& global);

	// Cleanup
	void shutdown_application();

	// Options
	core::config::Config config_from_path(const std::string& path);
}
