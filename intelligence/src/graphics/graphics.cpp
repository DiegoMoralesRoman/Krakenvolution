#include <atomic>
#include <functional>
#include <thread>

#include "easylogging/easylogging++.h"

#include "enviroment.hpp"
#include "nodes/node.hpp"
#include "rxcpp/subjects/rx-subject.hpp"
#include "graphics.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <tuple>

constexpr size_t WIDTH = 800;
constexpr size_t HEIGHT = 600;

void main_graphics_loop(
		const std::atomic<bool>& running, 
		core::nodes::GlobalContext& global, 
		std::function<void(core::graphics::Event)>&
	);

std::thread core::graphics::init_graphics(const std::atomic<bool>& running, nodes::GlobalContext& global, std::function<void(Event)>&& on_event) {
    auto handler = std::thread(main_graphics_loop, std::cref(running), std::ref(global), std::ref(on_event));
    return handler;
}

void init(core::graphics::Context& ctx) {
	ctx.win.setFramerateLimit(60);
};

std::vector<sf::Keyboard::Key> get_pressed_keys();
void send_mouse_button_events(const rxcpp::subscriber<sf::Mouse::Button>& mouse_btn_input_sub);
void handle_mouse_wheel_event(const sf::Event& event, const rxcpp::subscriber<std::tuple<sf::Mouse::Wheel, double>>& mouse_wheel_input_sub);

void main_graphics_loop(
		const std::atomic<bool>& running,
		core::nodes::GlobalContext& global,
		std::function<void(core::graphics::Event)>& on_event
) {
	rxcpp::subjects::subject<sf::Keyboard::Key> key_input;
	const auto& key_input_sub = key_input.get_subscriber();
	rxcpp::subjects::subject<sf::Mouse::Button> mouse_btn_input;
	const auto& mouse_btn_input_sub = mouse_btn_input.get_subscriber();
	rxcpp::subjects::subject<std::tuple<sf::Mouse::Wheel, double>> mouse_wheel_input;
	const auto& mouse_wheel_input_sub = mouse_wheel_input.get_subscriber();

	core::graphics::Context ctx {
		.win = sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Krakenvolution"),
		.key_input = key_input.get_observable(),
		.mouse_btn_input = mouse_btn_input.get_observable(),
		.mouse_wheel_input = mouse_wheel_input.get_observable()
	};
	init(ctx);

	core::graphics::impl::Enviroment env(ctx);

	while (running) {
		// ===========
		// Read events
		// ===========
		sf::Event ev;
		while (ctx.win.pollEvent(ev)) {
			handle_mouse_wheel_event(ev, mouse_wheel_input_sub);
			if (ev.type == sf::Event::Closed) {
				LOG(INFO) << "Sending close event";
				on_event(core::graphics::Event::CLOSE);
			}
		}

		for (auto& key : get_pressed_keys()) {
			key_input_sub.on_next(key);
		}
		send_mouse_button_events(mouse_btn_input_sub);

		ctx.win.clear();

		// Drawing enviroment
		env.update(ctx);
		env.draw(ctx);

		ctx.win.display();
	}
	LOG(INFO) << "Exiting main graphics execution loop";
}

std::vector<sf::Keyboard::Key> get_pressed_keys() {
    std::vector<sf::Keyboard::Key> pressedKeys;

    for (int key = sf::Keyboard::A; key != sf::Keyboard::KeyCount; ++key) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key))) {
            pressedKeys.push_back(static_cast<sf::Keyboard::Key>(key));
        }
    }

    return pressedKeys;
}

void send_mouse_button_events(const rxcpp::subscriber<sf::Mouse::Button>& mouse_btn_input_sub) {
    for (int button = sf::Mouse::Left; button != sf::Mouse::ButtonCount; ++button) {
        if (sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(button))) {
            mouse_btn_input_sub.on_next(static_cast<sf::Mouse::Button>(button));
        }
    }
}

void handle_mouse_wheel_event(const sf::Event& event, const rxcpp::subscriber<std::tuple<sf::Mouse::Wheel, double>>& mouse_wheel_input_sub) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            mouse_wheel_input_sub.on_next(std::make_tuple(sf::Mouse::VerticalWheel, event.mouseWheelScroll.delta));
        } else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel) {
            mouse_wheel_input_sub.on_next(std::make_tuple(sf::Mouse::HorizontalWheel, event.mouseWheelScroll.delta));
        }
    }
}
