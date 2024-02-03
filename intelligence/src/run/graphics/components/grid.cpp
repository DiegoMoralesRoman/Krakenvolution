#include "grid.hpp"
#include "easylogging/easylogging++.h"
#include <SFML/Graphics/Glsl.hpp>
#include <string>

const std::string frag_shader = R"(
#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

uniform float u_zoom;
uniform vec2 u_offset;

float stepGridScale(float zoom) {
    float baseScale = 10.0;
    float step = 2.0; // Define the stepping factor for grid scale

    // Calculate the scale factor based on zoom
    float scaleFactor = pow(step, floor(log(zoom) / log(step)));

    return baseScale / scaleFactor;
}

void main() {
    vec2 aspectCorrect = vec2(u_resolution.x / u_resolution.y, 1.0);
    
    // Apply zoom and offset transformations
    vec2 st = gl_FragCoord.xy / u_resolution.xy;
    st = (st - 0.5) * aspectCorrect * u_zoom + 0.5 + u_offset;

    // Dynamic grid scale based on zoom
    float grid_scale = stepGridScale(u_zoom); 

    // Grid calculation
    vec2 grid = mod(st * grid_scale, 1.0);
    float line_width = 0.03; 
    vec2 grid_mask = step(line_width, grid) * step(grid, vec2(1.0 - line_width));
    float line = grid_mask.x * grid_mask.y;

    // Circle calculations
    float circleRadius = 0.5 / u_zoom; 
    vec2 circleCenter = -u_offset; 
    float dist = distance(st, circleCenter);
    bool insideCircle = dist < circleRadius;

    // Set color based on grid or circle
    vec3 color = insideCircle ? vec3(1.0, 0.0, 0.0) : vec3(1.0) * line;
    
    gl_FragColor = vec4(color, 1.0);
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

	float zoom_factor = 1.0 / view_size.y; // Assuming square pixels for simplicity
	grid_shader.setUniform("u_zoom", zoom_factor);
	this->grid_shader.setUniform("u_offset", sf::Glsl::Vec2{ view_center.x, view_center.y });

	ctx.win.draw(this->backdrop, &this->grid_shader);
}
