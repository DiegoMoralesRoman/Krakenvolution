#pragma once
#include "graphics.hpp"

#include <rxcpp/subjects/rx-subject.hpp>
#include <topics/topics.hpp>

#include <SFML/Graphics/View.hpp>
#include <functional>

namespace run::graphics {
	struct Context {
		sf::RenderWindow win;
		sf::View view;
		rxcpp::observable<sf::Keyboard::Key> key_input;
		rxcpp::observable<sf::Mouse::Button> mouse_btn_input;
		rxcpp::observable<std::tuple<sf::Mouse::Wheel, double>> mouse_wheel_input;
		core::topics::GlobalContext& global;
		
		float dt;
		sf::Clock clock;
		const std::function<void(Event)>& send_event;
	};
}
