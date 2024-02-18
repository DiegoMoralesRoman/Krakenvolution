#pragma once

#include "connection_manager.hpp"
#include "sources/tcp/context.hpp"
#include "config/config.hpp"

#include <thread>
#include <optional>

namespace core::extensions::remote {
	struct SourceSharedContext {
		ConnectionManager conn_manager;
	};

	struct Context {
		sources::tcp::Context tcp_ctx;
		std::optional<std::thread> tcp_server_thread;
		SourceSharedContext source_shared_ctx;
		std::unordered_map<decltype(serial::Channel::UID), std::vector<rxcpp::composite_subscription>> channel_subscriptions;
	};
}
