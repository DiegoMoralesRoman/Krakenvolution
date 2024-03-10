#pragma once

#include "CLI11.hpp"
#include <memory>
#include <utility>

namespace sequencer::parser {
	struct Options {
		std::string topic;
		std::string ip;
		uint16_t port;
		std::string file;
	};

	std::unique_ptr<Options> add_options(CLI::App& app); 
	std::unique_ptr<Options> parse_cli(int argc, char* argv[]);
}
