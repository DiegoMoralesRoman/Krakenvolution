#include "parser.hpp"
#include <memory>

auto run::parser::add_options(CLI::App &app) -> std::unique_ptr<run::parser::Options> {
	auto options = std::make_unique<Options>();

	app.add_flag("--graphics", options->graphics,
			"Run with graphical interface");

	app.add_flag("--quiet", options->quiet,
			"Doesn't display the initial header");

	app.add_option("--config", options->config_path,
			"Path where the YAML configuration path is located \x1B[1m\x1B[32m[OPTIONAL]\x1B[0m")
	->check(CLI::ExistingFile);
	
	return options;
}
