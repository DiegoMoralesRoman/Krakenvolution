#include "config/config.hpp"
#include "run.hpp"

#include "easylogging/easylogging++.h"
#include <cstdlib>

void run::init_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, core::config::Config& cfg) {
	LOG(INFO) << "󰔟 Initializing nodes";
	auto empty_cfg = core::config::Config{};

	for (auto& an : nodes) {
		if (an.name == "") {
			LOG(ERROR) << " Found node without a name";
			exit(1);
		}

		auto node_cfg = cfg[an.name];
		if (static_cast<bool>(node_cfg)) {
			LOG(INFO) << "\x1b[1m\x1b[34m " << an.name << "\x1b[0m";
			an.node->setup(global, node_cfg);
		} else {
			LOG(INFO) << "\x1b[1m\x1b[34m " << an.name << " \x1B[22m(no config)\x1B[0m";
			an.node->setup(global, empty_cfg);
		}
	}

	LOG(INFO) << " Setup complete!";
}

void run::teardown_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, core::config::Config& cfg) {
	LOG(INFO) << "󰔟 Stopping nodes";
	auto empty_cfg = core::config::Config{};

	for (auto& an : nodes) {
			auto node_cfg = cfg[an.name];
			if (static_cast<bool>(node_cfg)) {
				LOG(INFO) << "\x1b[1m\x1b[34m " << an.name << "\x1b[0m";
				an.node->end(global, node_cfg);
			} else {
				LOG(INFO) << "\x1b[1m\x1b[34m " << an.name << " \x1B[22m(no config)\x1B[0m";
				an.node->end(global, empty_cfg);
			}
	}
	LOG(INFO) << " Nodes stopped";
}
