#include "CLI11.hpp"
#include "flag/flag.hpp"
#include "nodes/nodes.hpp"
#include "parser/parser.hpp"
#include "topics.hpp"
#include <easylogging/easylogging++.h>
#include <iostream>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[]) {
	START_EASYLOGGINGPP(argc, argv);

	core::GlobalContext global_context;
	auto nodes = nodes::create_all_nodes();

	CLI::App app("Krakenvolution");
	auto options = run::parser::add_options(app);

	auto flag = run::flag::flag();
	std::cout << flag << std::endl;

	CLI11_PARSE(app, argc, argv);
}
