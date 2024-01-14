#pragma once

#include "CLI11.hpp"
#include <memory>

namespace run::parser {
	struct Options {
		bool graphics;
	};

	std::unique_ptr<Options> add_options(CLI::App& app); 
}
