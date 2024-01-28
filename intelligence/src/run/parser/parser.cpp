#include "parser.hpp"
#include <memory>

std::unique_ptr<run::parser::Options> run::parser::add_options(CLI::App &app) {
	auto options = std::make_unique<Options>();

	app.add_flag("--graphics", options->graphics,
			"Run with graphical interface");

	app.add_flag("--quiet", options->quiet,
			"Doesn't display the initial header");
	
	return options;
}
