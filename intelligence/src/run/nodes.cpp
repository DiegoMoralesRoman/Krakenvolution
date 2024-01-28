#include "config/config.hpp"
#include "run.hpp"

#include "easylogging/easylogging++.h"
#include <cstdlib>

void run::init_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, core::config::Config& cfg) {
	LOG(INFO) << "Initializing nodes";
	auto empty_cfg = core::config::Config{};

	for (auto& an : nodes) {
		if (an.name == "") {
			LOG(ERROR) << "Found node without a name";
			exit(1);
		}

		auto node_cfg = cfg[an.name];
		if (static_cast<bool>(node_cfg)) {
			LOG(INFO) << "\t- " << an.name;
			an.node->setup(global, node_cfg);
		} else {
			LOG(INFO) << "\t- " << an.name << " (no config)";
			an.node->setup(global, empty_cfg);
		}
	}

	LOG(INFO) << "Setup complete!";
}

void run::teardown_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, core::config::Config& cfg) {
	LOG(INFO) << "Stopping nodes...";
	for (auto& an : nodes) {
		an.node->end(global, cfg);
		LOG(INFO) << "\t -" << an.name;
	}
}
