#include <atomic>
#include <csignal>
#include <functional>
#include <iostream>
#include <thread>

#include "CLI11.hpp"
#include "easylogging/easylogging++.h"

#include "logger.hpp"
#include "nodes/nodes.hpp"
#include "parser/parser.hpp"
#include "topics.hpp"
#include "flag/flag.hpp"
#include "graphics/graphics.hpp"

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
	// =================
	// Configure logging
	// =================
	START_EASYLOGGINGPP(argc, argv);
	auto logger_conf = run::log::get_config();
	el::Loggers::reconfigureLogger("default", logger_conf);

	// Main thread control variable
	std::atomic<bool> running = true;

	core::GlobalContext global_context (&running);
	auto nodes = nodes::create_all_nodes();

	// ===================
	// Add parsing options
	// ===================
	CLI::App app("Krakenvolution");
	auto options = run::parser::add_options(app);
	CLI11_PARSE(app, argc, argv);

	// ============
	// Initial flag
	// ============
	if (!options->quiet) {
		auto flag = run::flag::flag();
		std::cout << flag << std::endl << std::endl;
	}

	// ================
	// Running graphics
	// ================
	std::thread gui_thread;
	if (options->graphics) {
#ifdef HAS_SFML
		LOG(INFO) << "Launching GUI";

		// Window event callback function
		auto on_gui_event = [&running](core::graphics::Event ev) {
			switch (ev) {
				case core::graphics::Event::CLOSE:
				LOG(INFO) << "Closing GUI window";
				running = false;
				running.notify_all();
				break;
			}
		};

		gui_thread = core::graphics::init_graphics(running, global_context, on_gui_event);
#else
		LOG(WARNING) << "Application built without SFML support. Remove the --grahpics flag";
#endif
	}

	// =========================
	// Common running procedures
	// =========================
	// Stopping function
	sigint_func = [&](int signum) {
		LOG(INFO) << "Received closing signal from user <Ctrl-C>";
		running = false;
		running.notify_all();
	};
	std::signal(SIGINT, sigint_handler);

	// ================
	// Initialize nodes
	// ================
	LOG(INFO) << "Initializing nodes";
	for (auto& an : nodes) {
		an.node->setup(global_context);
		LOG(INFO) << "OK";
	}

	LOG(INFO) << "Setup complete!";

	running.wait(true); // Wait here until the program stops running

	// =========================================
	// Tearing down of the application resources
	// =========================================
	LOG(INFO) << "Stopping nodes...";
	for (auto& node : nodes) {
		node.node->end(global_context);
		LOG(INFO) << "OK";
	}

	LOG(INFO) << "Stopping threads...";
#ifdef HAS_SFML
	if (options->graphics) {
		gui_thread.join();
		LOG(INFO) << "GUI stopped";
	}
#endif

	LOG(INFO) << "Closing application ";
	return 0;
}
