#pragma once

#include "../parser/parser.hpp"

#include <functional>
#include <serial/channel.hpp>

#include <tuple>

namespace sender::tui {
	using StartFn = std::function<void()>;
	using ExitFn = std::function<void()>;

	auto run(const parser::Options& options) -> std::tuple<StartFn, ExitFn>;
}
