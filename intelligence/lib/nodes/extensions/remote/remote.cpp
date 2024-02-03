#include "remote.hpp"
#include "easylogging/easylogging++.h"
#include "nodes/node.hpp"

#include "sources/tcp/tcp.hpp"

core::nodes::ApplicationNode core::extensions::remote::create_node() {
	return nodes::ApplicationNode {
		.node = nodes::create_node<Context>(setup, end),
		.name = "remote"
	};
}

void core::extensions::remote::setup(::core::topics::GlobalContext &global, Context &ctx, const ::core::config::Config &cfg) {
	LOG(INFO) << " Initializing remote channels";

	ctx.source_shared_ctx.conn_manager.on_new_channel([&global, &ctx](const Channel& channel) {
		LOG(DEBUG) << "󰟥 Created channel [" << ctx.source_shared_ctx.conn_manager.find_managed_channel(channel)->get().name << ']';
		channel.tx.on_next("Value");
		channel.rx
			.take_until(global.stop_signal)
			.subscribe([](const std::string& data) {

			});
	});

	ctx.source_shared_ctx.conn_manager.on_removed_channel([&global, &ctx](const Channel& channel) {
		LOG(DEBUG) << "󰟦 Removed channel [" << ctx.source_shared_ctx.conn_manager.find_managed_channel(channel)->get().name << ']';
	});

	// TCP
	if (cfg["tcp"].IsDefined()) {
		ctx.tcp_server_thread = sources::tcp::init_server(global, ctx.tcp_ctx, ctx.source_shared_ctx, cfg["tcp"]);
	}
	if (cfg["uart"].IsDefined()) {
		
	}
}

void core::extensions::remote::end(::core::topics::GlobalContext &global, Context &ctx, const ::core::config::Config &cfg) {
	if (ctx.tcp_server_thread.has_value()) ctx.tcp_server_thread->join();
}
