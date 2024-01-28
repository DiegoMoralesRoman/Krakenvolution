#include "config/config.hpp"
#include "run.hpp"

#include "easylogging/easylogging++.h"

void run::init_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, const core::config::Config& cfg) {
	LOG(INFO) << "Initializing nodes";
	for (auto& an : nodes) {
		an.node->setup(global, cfg);
		LOG(INFO) << "OK";
	}

	LOG(INFO) << "Setup complete!";
}

void run::teardown_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global, const core::config::Config& cfg) {
	LOG(INFO) << "Stopping nodes...";
	for (auto& an : nodes) {
		an.node->end(global, cfg);
		LOG(INFO) << "OK";
	}
}
