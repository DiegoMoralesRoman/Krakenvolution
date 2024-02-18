#include <easylogging/easylogging++.h>
#include "run/logger.hpp"

#include "sender/parser/parser.hpp"
#include "sender/tui/tui.hpp"

INITIALIZE_EASYLOGGINGPP

int main(int argc, char** argv) {
	START_EASYLOGGINGPP(argc, argv);
	auto logger_conf = run::log::get_config();
	el::Loggers::reconfigureLogger("default", logger_conf);

	auto options = sender::parser::parse_cli(argc, argv);
	if (options == nullptr) {
		LOG(ERROR) << "󰆍 Failed to process CLI args";
		exit(1);
	};

	// Launch GUI
	sender::tui::run();

	LOG(INFO) << " Closing application";
}
