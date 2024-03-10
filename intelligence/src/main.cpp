#include "easylogging/easylogging++.h"

#include "run/application.hpp"
#include "run/run.hpp"

#include "extensions.hpp"
#include <nodes.hpp>

INITIALIZE_EASYLOGGINGPP

#include <nodes/node_definitions.hpp> // For static initialization

auto main(int argc, char* argv[]) -> int {
	run::run(argc, argv);
	run::shutdown_application();

	LOG(INFO) << " Closing application \x1B[1m\x1B[38;2;245;169;184m>^.^<\x1B[0m";

	return 0;
}
