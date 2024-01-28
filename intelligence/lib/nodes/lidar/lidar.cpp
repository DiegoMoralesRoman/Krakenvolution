#include "lidar.hpp"
#include "easylogging/easylogging++.h"
#include "nodes/node.hpp"
#include "rxcpp/operators/rx-observe_on.hpp"
#include "rxcpp/rx-observable.hpp"
#include "topics.hpp"
#include <chrono>
#include <functional>
#include <future>

void loop_worker(core::topics::GlobalContext&, core::nodes::lidar::Context&);

core::nodes::ApplicationNode core::nodes::lidar::create_node() {
	return core::nodes::ApplicationNode{
		.node = core::nodes::create_node<lidar::Context>(setup, end),
	};
}

void core::nodes::lidar::setup(core::topics::GlobalContext& global, lidar::Context &ctx, const config::Config& cfg) {
	global.topics.test.get_observable()
		.observe_on(rxcpp::observe_on_new_thread())
		.take_until(global.stop_signal)
		.subscribe([](int value) {
			LOG(DEBUG) << "Value: " << value;
		});

	ctx.loop_future = std::async(
		loop_worker,
		std::ref(global),
		std::ref(ctx)
	);
}

void core::nodes::lidar::end(core::topics::GlobalContext &global, lidar::Context &ctx, const config::Config& cfg) {
	ctx.loop_future.wait();
}

void loop_worker(core::topics::GlobalContext& global, core::nodes::lidar::Context& ctx) {
	auto obs = rxcpp::observable<>::interval(std::chrono::milliseconds(500))
		.take_until(global.stop_signal)
		.subscribe([&](long iter) {
				// Iteration loop
				global.topics.test.get_subscriber().on_next(iter);
			});
}

