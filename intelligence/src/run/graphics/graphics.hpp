#pragma once

#include <topics/topics.hpp>

#include <functional>
#include <thread>

#include <SFML/Graphics.hpp>

namespace run::graphics {
	enum class Event {
		CLOSE
	};

	struct WinDimensions {
		size_t width;
		size_t height;
	};

	auto init_graphics(
			const WinDimensions& win_size,
			core::topics::GlobalContext& global,
			const std::function<void(Event)>&
		) -> std::thread;
}
