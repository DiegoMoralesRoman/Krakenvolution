#pragma once

#include "context.hpp"
#include <nodes/extensions/remote/sources/uart/err.hpp>

#include <config/config.hpp>
#include <topics/topics.hpp>
#include <nodes/extensions/remote/context.hpp>

#include <optional>
#include <thread>
#include <expected>

namespace core::extensions::remote::sources::uart {
	using BindPortResult = std::expected<std::thread, BindPortErr>;

	auto init_server(
			topics::GlobalContext& global,
			uart::Context& ctx,
			SourceSharedContext& shared_ctx,
			const config::Config& cfg
		) -> std::optional<std::thread>;
}
