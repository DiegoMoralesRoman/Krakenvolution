#pragma once

#include "topics/topics.hpp"
#include <atomic>
#include <functional>
#include <thread>

#include <SFML/Graphics.hpp>

namespace run::graphics {
	enum class Event {
		CLOSE
	};

	std::thread init_graphics(
			const std::atomic<bool>& running,
			core::topics::GlobalContext& global,
			const std::function<void(Event)>&
		);
}
