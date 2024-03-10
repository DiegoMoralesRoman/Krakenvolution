#include "run.hpp"

#include <CLI11.hpp>
#include <easylogging/easylogging++.h>

#include "logger.hpp"

#include <cstdint>
#include <memory>

auto run::config_easylogging(int argc, char* argv[]) -> void {
	START_EASYLOGGINGPP(argc, argv);
	auto logger_conf = run::log::get_config();
	el::Loggers::reconfigureLogger("default", logger_conf);
}

auto parse(int argc, char* argv[], CLI::App& app)  -> std::optional<int32_t> {
	CLI11_PARSE(app, argc, argv);
	// return INT32_MAX;
	return std::nullopt;
}

auto run::parse_cli(int argc, char* argv[])  -> std::unique_ptr<run::parser::Options> {
	CLI::App app("Krakenvolution");
	auto options = run::parser::add_options(app);
	if (parse(argc, argv, app).has_value())
		return nullptr;
	return options;
}
