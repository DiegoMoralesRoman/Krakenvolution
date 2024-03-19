#pragma once

#include "connection_manager.hpp"
#include "nodes/extensions/remote/sources/uart/context.hpp"
#include "sources/tcp/context.hpp"

#include <thread>
#include <optional>
#include <utility>

namespace core::extensions::remote {
	struct SourceSharedContext {
		ConnectionManager conn_manager;
	};

	struct Context {
		sources::tcp::Context tcp_ctx;
		sources::uart::Context uart_ctx;
		std::optional<std::thread> tcp_server_thread;
		std::optional<std::thread> uart_server_thread;

		SourceSharedContext source_shared_ctx;
		std::unordered_map<
				decltype(serial::Channel<>::UID),
				std::vector<std::pair<std::string, rxcpp::composite_subscription>>
			> channel_subscriptions;
	};
}
