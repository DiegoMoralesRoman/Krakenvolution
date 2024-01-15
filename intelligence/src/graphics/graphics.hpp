#pragma once

#include "topics.hpp"
#include <atomic>
#include <functional>
#include <thread>

#include <SFML/Graphics.hpp>

namespace core::graphics {
	enum class Event {
		CLOSE
	};

	struct Context {
		sf::RenderWindow win;
		rxcpp::observable<sf::Keyboard::Key> input;
	};

	std::thread init_graphics(
			const std::atomic<bool>& running,
			topics::GlobalContext& global,
			std::function<void(Event)>&&
		);
}
