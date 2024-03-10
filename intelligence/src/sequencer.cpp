#include "run/logger.hpp"
#include "sequencer/parser/parser.hpp"
#include <cstdlib>
#include <networking/connection.hpp>
#include <networking/err.hpp>

#include <easylogging/easylogging++.h>

#include <functional>
#include <csignal>

INITIALIZE_EASYLOGGINGPP

std::function<void(int signum)> sigint_func;
auto sigint_handler(int signum) -> void {
	sigint_func(signum);
}

auto main(int argc, char** argv) -> int {
	START_EASYLOGGINGPP(argc, argv);

	auto logger_conf = run::log::get_config();
	el::Loggers::reconfigureLogger("default", logger_conf);

	auto options = sequencer::parser::parse_cli(argc, argv);
	if (options == nullptr) {
		LOG(ERROR) << "󰆍 Failed to process CLI args";
		exit(1);
	};

	core::net::Events ev;
	ev.on_disconnected([]{
		LOG(INFO) << "󰈂 Disconnected from core";
	});

	// Run application
	LOG(INFO) << "󰈁 Connecting to " << options->ip << ":" << options->port;
	auto conn_result = core::net::connect(ev, options->ip, options->port);

	if (not conn_result.has_value()) {
		LOG(ERROR) << core::net::err_repr(conn_result.error());
		exit(1);
	}
	auto& conn = conn_result.value();
	LOG(INFO) << " Connection successful";


	sigint_func = [&](int signum) {
		LOG(INFO) << "Received interrupt signal from user <Ctrl-C>";
		LOG(INFO) << "󰈂 Closing connection";
		conn.close();
	};
	std::signal(SIGINT, sigint_handler);

	if (conn.conn_thread.joinable()) { conn.conn_thread.join(); }
	LOG(INFO) << " Closing application";
}
