#include "lidar.hpp"
#include "easylogging/easylogging++.h"
#include "nodes/node.hpp"
#include "rxcpp/operators/rx-observe_on.hpp"
#include "rxcpp/rx-observable.hpp"
#include <chrono>
#include <functional>
#include <future>

void loop_worker(core::topics::GlobalContext&, core::nodes::lidar::Context&);

core::nodes::ApplicationNode core::nodes::lidar::create_node() {
	return core::nodes::ApplicationNode{
		.node = core::nodes::create_node<lidar::Context>(setup, end),
		.name = "lidar"
	};
}

void core::nodes::lidar::setup(core::topics::GlobalContext& global, lidar::Context &ctx, const config::Config& cfg) {

}

void core::nodes::lidar::end(core::topics::GlobalContext &global, lidar::Context &ctx, const config::Config& cfg) {

}


