#pragma once

#include <future>

namespace core::nodes::lidar {
	struct Context {
		std::future<void> loop_future;
	};
}
