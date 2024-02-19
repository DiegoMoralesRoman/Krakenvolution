#include "enviroment.hpp"
#include "grid.hpp"

constexpr float MOVE_MULT = 0.05;
constexpr float ZOOM_MULT = 0.1;

run::graphics::components::Enviroment::Enviroment(run::graphics::Context& ctx)
	: grid(ctx) {

	ctx.mouse_wheel_input.subscribe([&](const auto& ev) {
		const auto& [wheel, amount] = ev;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
			if (wheel == sf::Mouse::VerticalWheel) {
				const sf::Vector2i mousePixelPos = sf::Mouse::getPosition(ctx.win);
				const sf::Vector2f mouseWorldPos = ctx.win.mapPixelToCoords(mousePixelPos, ctx.view);

				const float zoomFactor = 1.0f - (amount * ZOOM_MULT);
				ctx.view.zoom(zoomFactor);

				const sf::Vector2f newMouseWorldPos = ctx.win.mapPixelToCoords(mousePixelPos, ctx.view);
				ctx.view.move(mouseWorldPos - newMouseWorldPos);
			}
		} else {
			const float relative_move_ammount = ctx.view.getSize().y * amount * MOVE_MULT;
			switch (wheel) {
				case sf::Mouse::VerticalWheel:
					ctx.view.move(0, relative_move_ammount);
					break;
				case sf::Mouse::HorizontalWheel:
					ctx.view.move(relative_move_ammount, 0);
					break;
			}
		}
	});

}

auto run::graphics::components::Enviroment::update(Context& ctx) -> void {
	grid.update(ctx);
}

auto run::graphics::components::Enviroment::render(Context& ctx) -> void {
	grid.render(ctx);
}
