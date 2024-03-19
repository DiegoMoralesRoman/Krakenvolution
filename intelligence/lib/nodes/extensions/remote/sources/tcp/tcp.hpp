#pragma once
#include <expected>
#include <thread>

#include <config/config.hpp>
#include "nodes/extensions/remote/context.hpp"
#include "topics/topics.hpp"

#include "err.hpp"
#include "context.hpp"

namespace core::extensions::remote::sources::tcp {
	using CreateServerResult = std::expected<std::thread, CreateServerErr>;

	auto create_server(topics::GlobalContext& global, tcp::Context& ctx, SourceSharedContext& shared_ctx, const config::Config& cfg) -> CreateServerResult;
	auto init_server(topics::GlobalContext& global, tcp::Context& ctx, SourceSharedContext& shared_ctx, const config::Config& cfg) -> std::optional<std::thread>;
}
