#include "test.hpp"
#include "easylogging/easylogging++.h"
#include "nodes/node.hpp"
#include "rxcpp/operators/rx-observe_on.hpp"
#include <chrono>
#include <functional>

core::nodes::ApplicationNode core::nodes::test::create_node() {
	return ApplicationNode {
		.node = core::nodes::create_node<Context>(setup, end),
		.name = "test"
	};
}

void loop(core::nodes::GlobalContext& global, core::nodes::test::Context& ctx) {
	auto obs = rxcpp::observable<>::interval(std::chrono::seconds(1))
		.take_until(global.stop_signal)
		.subscribe([&global](auto iter) {
			Person new_person;
			new_person.set_name("Laura");
			new_person.set_id(iter);
			global.topics.person.get_subscriber().on_next(new_person);
		});
}

// Main node functions
void core::nodes::test::setup(::core::topics::GlobalContext &global, test::Context &ctx, const ::core::config::Config &cfg) {
	ctx.loop_thread = std::thread(loop, std::ref(global), std::ref(ctx));

	global.topics.person.get_observable()
		.observe_on(rxcpp::observe_on_new_thread())
		.take_until(global.stop_signal)
		.subscribe([](const Person& person) {
			LOG(DEBUG) << "[TEST] New person: " << person.name() << ", " << person.id();
		});

}

void core::nodes::test::end(::core::topics::GlobalContext &global, test::Context &ctx, const ::core::config::Config &cfg) {
	ctx.loop_thread.join();
}
