#include <nodes/node_definitions.hpp>
#include <cstdio>

#include "easylogging/easylogging++.h"
#include "rxcpp/operators/rx-observe_on.hpp"
#include <chrono>
#include <functional>

CREATE_NODE(test)

auto loop(core::topics::GlobalContext& global, core::nodes::test::Context& ctx) -> void {
	// auto obs = rxcpp::observable<>::interval(std::chrono::seconds(1))
	// 	.take_until(global.stop_signal)
	// 	.subscribe([&global](auto iter) {
	// 		Person new_person;
	// 		new_person.set_name("Laura");
	// 		new_person.set_id(iter);
	// 		global.topics.person.get_subscriber().on_next(new_person);
	// 	});
}

// Main node functions
auto core::nodes::test::setup(::core::topics::GlobalContext &global, test::Context &ctx, const ::core::config::Config &cfg) -> void {
	ctx.loop_thread = std::thread(loop, std::ref(global), std::ref(ctx));

	global.topics.person.get_observable()
		.observe_on(rxcpp::observe_on_new_thread())
		.take_until(global.stop_signal)
		.subscribe([](const Person& person) {
			LOG(DEBUG) << "[TEST] New person: " << person.name() << ", " << person.id();
		});
}

auto core::nodes::test::end(::core::topics::GlobalContext &global, test::Context &ctx, const ::core::config::Config &cfg) -> void {
	ctx.loop_thread.join();
}
