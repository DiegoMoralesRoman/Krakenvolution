#pragma once

#include "graphics.hpp"
#include "nodes/node.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/View.hpp>
#include <functional>
#include <rxcpp/rx.hpp>

namespace run::graphics {
	struct Context {
		sf::RenderWindow win;
		sf::View view;
		rxcpp::observable<sf::Keyboard::Key> key_input;
		rxcpp::observable<sf::Mouse::Button> mouse_btn_input;
		rxcpp::observable<std::tuple<sf::Mouse::Wheel, double>> mouse_wheel_input;
		core::nodes::GlobalContext& global;
		
		float dt;
		sf::Clock clock;
		const std::function<void(Event)>& send_event;
	};
}
