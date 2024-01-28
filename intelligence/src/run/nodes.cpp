#include "run.hpp"

#include "easylogging/easylogging++.h"

void run::init_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global) {
	LOG(INFO) << "Initializing nodes";
	for (auto& an : nodes) {
		an.node->setup(global);
		LOG(INFO) << "OK";
	}

	LOG(INFO) << "Setup complete!";
}

void run::teardown_nodes(std::vector<core::nodes::ApplicationNode>& nodes, core::topics::GlobalContext& global) {
	LOG(INFO) << "Stopping nodes...";
	for (auto& node : nodes) {
		node.node->end(global);
		LOG(INFO) << "OK";
	}
}
