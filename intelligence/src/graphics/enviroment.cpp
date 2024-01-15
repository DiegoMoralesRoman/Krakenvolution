#include "enviroment.hpp"
#include "easylogging/easylogging++.h"
#include "graphics.hpp"

core::graphics::impl::Enviroment::Enviroment(Context& ctx) {
	ctx.mouse_wheel_input.subscribe([this](const auto& ev) {
		const auto& [dir, value] = ev;
		switch (dir) {
			case sf::Mouse::VerticalWheel:
				this->circle.move(0.0, value);
				break;
			case sf::Mouse::HorizontalWheel:
				this->circle.move(value, 0.0);
				break;
		}
	});

	this->circle.setPosition(ctx.win.getSize().x / 2.0, ctx.win.getSize().y / 2.0);
	this->circle.setFillColor(sf::Color::Green);
}

void core::graphics::impl::Enviroment::update(Context& ctx) {
}

void core::graphics::impl::Enviroment::draw(Context& ctx) const {
	ctx.win.draw(this->circle);
}
