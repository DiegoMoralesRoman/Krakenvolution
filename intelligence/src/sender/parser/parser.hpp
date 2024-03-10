#pragma once

#include "CLI11.hpp"
#include <memory>
#include <utility>

namespace sender::parser {
	struct Options {
		std::string topic;
		std::string ip;
		uint16_t port;
	};

	std::unique_ptr<Options> add_options(CLI::App& app); 
	std::unique_ptr<parser::Options> parse_cli(int argc, char* argv[]);
}
