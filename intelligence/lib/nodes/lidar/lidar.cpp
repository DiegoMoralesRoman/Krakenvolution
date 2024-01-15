#include "lidar.hpp"
#include "easylogging/easylogging++.h"
#include "nodes/node.hpp"
#include "rxcpp/operators/rx-observe_on.hpp"
#include "rxcpp/rx-observable.hpp"
#include "topics.hpp"
#include <chrono>
#include <functional>
#include <future>

void loop_worker(core::GlobalContext&, nodes::lidar::Context&);

core::ApplicationNode nodes::lidar::create_node() {
	return core::ApplicationNode{
		.node = core::create_node<lidar::Context>(setup, end),
	};
}

void nodes::lidar::setup(core::GlobalContext& global, lidar::Context &ctx) {
	global.topics.test.get_observable()
		.observe_on(rxcpp::observe_on_new_thread())
		.subscribe([](int value) {
			LOG(DEBUG) << "Value: " << value;
		});

	ctx.loop_handler = std::async(
		loop_worker,
		std::ref(global),
		std::ref(ctx)
	);
}

void nodes::lidar::end(::core::GlobalContext &global, lidar::Context &ctx) {
	ctx.loop_handler.wait();
}

void loop_worker(core::GlobalContext& global, nodes::lidar::Context& ctx) {
	auto obs = rxcpp::observable<>::interval(std::chrono::milliseconds(500))
		.take_until(global.stop_signal)
		.subscribe([&](long iter) {
				// Iteration loop
				global.topics.test.get_subscriber().on_next(iter);
			});
}

