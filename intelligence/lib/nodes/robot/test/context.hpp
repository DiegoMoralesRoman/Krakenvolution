#pragma once

#include <thread>

namespace core::nodes::test {
	struct Context {
		std::thread loop_thread;
	};
}
