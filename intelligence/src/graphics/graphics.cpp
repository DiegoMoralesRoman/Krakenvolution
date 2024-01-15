#include <atomic>
#include <functional>
#include <thread>

#include "easylogging/easylogging++.h"

#include "enviroment.hpp"
#include "topics.hpp"
#include "graphics.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

constexpr size_t WIDTH = 800;
constexpr size_t HEIGHT = 600;

void main_graphics_loop(
		const std::atomic<bool>& running, 
		core::GlobalContext& global, 
		std::function<void(core::graphics::Event)>&
	);

std::thread core::graphics::init_graphics(const std::atomic<bool>& running, GlobalContext& global, std::function<void(Event)>&& on_event) {
    auto handler = std::thread(main_graphics_loop, std::cref(running), std::ref(global), std::ref(on_event));
    return handler;
}

void init(core::graphics::Context& ctx) {
	ctx.win.setFramerateLimit(60);
};

void main_graphics_loop(
		const std::atomic<bool>& running,
		core::GlobalContext& global,
		std::function<void(core::graphics::Event)>& on_event
) {
	core::graphics::Context ctx {
		.win = sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Krakenvolution")
	};
	init(ctx);

	core::graphics::impl::Enviroment env;

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

		ctx.win.clear();

		// Drawing enviroment
		env.update(ctx);
		env.draw(ctx);

		ctx.win.display();
	}
	LOG(INFO) << "Exiting main graphics execution loop";
}
