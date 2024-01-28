#include <csignal>
#include <functional>
#include <thread>

#include "easylogging/easylogging++.h"

#include "nodes/nodes.hpp"
#include "run/flag/flag.hpp"

#include "run/run.hpp"

/**
 * TODO: have to create extensions folder (I'll need it for networking, serial, etc.)
 * TODO: add serialization and deserialization library to this
 * 	- Has to have something like JSON and BIN output (maybe protobuf if impl for Arduino)
 */

INITIALIZE_EASYLOGGINGPP

// This has to be this way because the signal function only takes a void(*)() function pointer
std::function<void(int signum)> sigint_func; // <-- Real callback that will be called upon SIGINT
void sigint_handler(int signum) {
	sigint_func(signum);
}

int main(int argc, char* argv[]) {
	run::config_easylogging(argc, argv);

	// Main thread control variable
	std::atomic<bool> running = true;

	core::topics::GlobalContext global_context (&running);
	auto nodes = core::nodes::create_all_nodes();

	auto options = run::parse_cli(argc, argv);
	if (options == nullptr) return 0;

	// Interpreting command line options
	if (!options->quiet) {
		auto flag = run::flag::flag();
		std::cout << flag << std::endl << std::endl;
	}

	std::optional<std::thread> gui_thread;
	if (options->graphics) {
		gui_thread = run::init_graphics_thread(running, global_context);
	}

	// Configure SIGINT function
	sigint_func = [&](int signum) {
		LOG(INFO) << "Received closing signal from user <Ctrl-C>";
		running = false;
		running.notify_all();
	};
	std::signal(SIGINT, sigint_handler);

	run::init_nodes(nodes, global_context);

	running.wait(true); // Wait here until the program stops running

	run::teardown_nodes(nodes, global_context);

	LOG(INFO) << "Stopping threads...";
	if (options->graphics && gui_thread.has_value()) {
		gui_thread->join();
		LOG(INFO) << "GUI stopped";
	}

	run::shutdown_application();
	LOG(INFO) << "Closing application ";

	return 0;
}
