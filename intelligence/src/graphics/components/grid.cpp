#include "grid.hpp"

run::graphics::components::Grid::Grid(run::graphics::Context& ctx) {
	circle.setPosition(0, 0);
	circle.setRadius(0.5);
}

void run::graphics::components::Grid::update(Context& ctx) {
}

void run::graphics::components::Grid::render(Context& ctx) {
	ctx.win.draw(this->circle);
}
