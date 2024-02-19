#include "config/config.hpp"
#include "run.hpp"

#include "easylogging/easylogging++.h"
#include <cstdlib>
#include <format>
#include <variant>

auto repr_load_err(const core::config::LoadErr& err) -> std::string {
	struct {
		std::string operator()(const core::config::FileNotFoundErr& err) {
			return std::format("Configuration file not found: {}", err.file);
		}

		std::string operator()(const core::config::ParseErr& err) {
			return std::format("Failed to parse configuration file: {}\nat {}", err.reason, err.where);
		}

		std::string operator()(const std::string& err) {
			return std::format("Unexpected: ", err);
		}
	} visitor;
	return std::visit(visitor, err);
}

auto run::config_from_path(const std::string &path) -> core::config::Config {
	LOG(INFO) << "󰔟  Loading configuration from " << path;
	const auto config = core::config::load_config(path);
	if (config.has_value()) {
		LOG(INFO) << " Configuration loaded";
		return std::move(*config);
	} else {
		LOG(ERROR) << "❌Failed to load configuration";
		LOG(ERROR) << repr_load_err(config.error());
		exit(1);
	}
}
