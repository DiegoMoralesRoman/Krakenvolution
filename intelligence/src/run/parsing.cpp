#include "CLI11.hpp"
#include "easylogging/easylogging++.h"
#include "run.hpp"
#include "logger.hpp"

#include <cstdint>
#include <memory>

void run::config_easylogging(int argc, char* argv[]) {
	START_EASYLOGGINGPP(argc, argv);
	auto logger_conf = run::log::get_config();
	el::Loggers::reconfigureLogger("default", logger_conf);
}

std::optional<int32_t> parse(int argc, char* argv[], CLI::App& app) {
	CLI11_PARSE(app, argc, argv);
	// return INT32_MAX;
	return std::nullopt;
}

std::unique_ptr<run::parser::Options> run::parse_cli(int argc, char* argv[]) {
	CLI::App app("Krakenvolution");
	auto options = run::parser::add_options(app);
	if (parse(argc, argv, app).has_value())
		return nullptr;
	return options;
}
