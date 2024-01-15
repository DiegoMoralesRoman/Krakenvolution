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
void main_graphics_loop(
		const std::atomic<bool>& running,
		core::nodes::GlobalContext& global,
		std::function<void(core::graphics::Event)>& on_event
) {
	rxcpp::subjects::subject<sf::Keyboard::Key> input;
	const auto& input_sub = input.get_subscriber();
	core::graphics::Context ctx {
		.win = sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Krakenvolution"),
		.input = input.get_observable()
	};
	init(ctx);

	core::graphics::impl::Enviroment env(ctx);

	while (running) {
		// ===========
		// Read events
		// ===========
		sf::Event ev;
		while (ctx.win.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				LOG(INFO) << "Sending close event";
				on_event(core::graphics::Event::CLOSE);
			}
		}

		for (auto& key : get_pressed_keys()) {
			input_sub.on_next(key);
		}

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
