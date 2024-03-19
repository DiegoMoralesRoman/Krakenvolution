#include "test.hpp"

#include <cstdio>

#include "easylogging/easylogging++.h"
#include "rxcpp/operators/rx-observe_on.hpp"
#include <chrono>
#include <functional>

auto loop(core::topics::GlobalContext& global, core::nodes::test::Context& ctx) -> void {
	auto obs = rxcpp::observable<>::interval(std::chrono::seconds(1))
		.take_until(global.stop_signal)
		.subscribe([&global](auto iter) {
			Led led;
			led.set_state(iter % 2 != 0);
			// led.set_state(true);
			global.topics.led.get_subscriber().on_next(led);
		});
}

// Main node functions
auto core::nodes::test::setup(::core::topics::GlobalContext &global, test::Context &ctx, const ::core::config::Config &cfg) -> void {
	ctx.loop_thread = std::thread(loop, std::ref(global), std::ref(ctx));

	global.topics.led.get_observable().subscribe([](const Led& msg){
		LOG(DEBUG) << "Received LED: " << msg.state();
	});
}

auto core::nodes::test::end(::core::topics::GlobalContext &global, test::Context &ctx, const ::core::config::Config &cfg) -> void {
	ctx.loop_thread.join();
}
