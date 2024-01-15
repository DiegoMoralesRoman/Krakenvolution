#pragma once

#include <future>
#include <thread>

namespace nodes::lidar {
	struct Context {
		std::future<void> loop_handler;
	};
}
