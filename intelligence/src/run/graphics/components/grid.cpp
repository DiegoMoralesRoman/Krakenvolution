#include "grid.hpp"
#include "easylogging/easylogging++.h"
#include <string>

const std::string frag_shader = R"(
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;

void main() {
	vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;

    vec3 color = vec3(0.);
    color = vec3(st.x,st.y,abs(sin(u_time)));

    gl_FragColor = vec4(color,1.0);
}
)";

run::graphics::components::Grid::Grid(run::graphics::Context& ctx) {
	if (not this->grid_shader.loadFromMemory(frag_shader, sf::Shader::Fragment)) {
		LOG(ERROR) << "Failed to load fragment shader for grid";
		ctx.send_event(Event::CLOSE);
	}

	this->backdrop.setPosition(0, 0);
	this->backdrop.setSize({0.5, 0.5});

	this->grid_shader.setUniform("u_resolution", sf::Glsl::Vec2{
		static_cast<float>(ctx.win.getSize().x),
		static_cast<float>(ctx.win.getSize().y)
	});
}

void run::graphics::components::Grid::update(Context& ctx) {
	this->grid_shader.setUniform("u_time", ctx.clock.getElapsedTime().asSeconds());
}

void run::graphics::components::Grid::render(Context& ctx) {
	const auto view_size = ctx.win.getView().getSize();
	const auto view_center = ctx.win.getView().getCenter();
	this->backdrop.setSize(view_size);
	this->backdrop.setOrigin(view_size.x / 2.0f, view_size.y / 2.0f);
	this->backdrop.setPosition(view_center);

	ctx.win.draw(this->backdrop, &this->grid_shader);
}
