#include "enviroment.hpp"
#include "graphics.hpp"

core::graphics::impl::Enviroment::Enviroment(Context& ctx) {
	ctx.input.subscribe([this](sf::Keyboard::Key key) {
		switch (key) {
			case sf::Keyboard::Key::W:
				this->circle.move(0.0, -1.0);
				break;
			case sf::Keyboard::Key::A:
				this->circle.move(-1.0, 0.0);
				break;
			case sf::Keyboard::Key::S:
				this->circle.move(0.0, 1.0);
				break;
			case sf::Keyboard::Key::D:
				this->circle.move(1.0, 0.0);
				break;
			default:
				break;
		}
	});

	this->circle.setPosition(ctx.win.getSize().x / 2, ctx.win.getSize().y / 2);
}

void core::graphics::impl::Enviroment::update(Context& ctx) {
	this->circle.setFillColor(sf::Color::Green);
}

void core::graphics::impl::Enviroment::draw(Context& ctx) const {
	ctx.win.draw(this->circle);
}
