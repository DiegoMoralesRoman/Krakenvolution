#pragma once

#include <thread>

namespace nodes::lidar {
	struct Context {
		std::thread reader_thread;
	};
}
