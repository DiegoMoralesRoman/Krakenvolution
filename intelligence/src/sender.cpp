#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <easylogging/easylogging++.h>
#include "run/logger.hpp"

#include <networking/err.hpp>
#include "sender/parser/parser.hpp"
#include "sender/tui/tui.hpp"
#include <networking/connection.hpp>

#include <format>
#include <functional>

INITIALIZE_EASYLOGGINGPP

std::function<void(int signum)> sigint_func;
auto sigint_handler(int signum) -> void {
	sigint_func(signum);
}

auto main(int argc, char** argv) -> int {
	START_EASYLOGGINGPP(argc, argv);
	auto logger_conf = run::log::get_config();
	el::Loggers::reconfigureLogger("default", logger_conf);

	auto options = sender::parser::parse_cli(argc, argv);
	if (options == nullptr) {
		LOG(ERROR) << "󰆍 Failed to process CLI args";
		exit(1);
	};

	// Configure SIGINT handler
	sigint_func = [&](int signum) {
		LOG(DEBUG) << "SIGINT";
	};
	std::signal(SIGINT, sigint_handler);

	auto [start_tui, stop_tui] = sender::tui::run(*options.get());

	start_tui(); // Blocking

	LOG(INFO) << " Closing application";
}
