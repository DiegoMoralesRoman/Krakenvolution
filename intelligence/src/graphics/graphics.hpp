#pragma once

#include "rxcpp/rx-observable.hpp"
#include "topics.hpp"
#include <atomic>
#include <functional>
#include <thread>

#include <SFML/Graphics.hpp>
#include <tuple>

namespace core::graphics {
	enum class Event {
		CLOSE
	};

	struct Context {
		sf::RenderWindow win;
		rxcpp::observable<sf::Keyboard::Key> key_input;
		rxcpp::observable<sf::Mouse::Button> mouse_btn_input;
		rxcpp::observable<std::tuple<sf::Mouse::Wheel, double>> mouse_wheel_input;
	};

	std::thread init_graphics(
			const std::atomic<bool>& running,
			topics::GlobalContext& global,
			std::function<void(Event)>&&
		);
}
