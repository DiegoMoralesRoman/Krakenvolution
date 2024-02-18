#include "parser.hpp"
#include <memory>

std::unique_ptr<sender::parser::Options> sender::parser::add_options(CLI::App &app) {
	auto options = std::make_unique<Options>();

	app.footer("MsgSender by Laura Morales Román");

	app.require_subcommand();
	auto sub = app.add_subcommand("chat", "Acts as a chat with the topic");

	// sub->add_option("--topic", options->topic,
	// 			 "Specifies the topic that the messages will be sent and received from");
	
	return options;
}

std::optional<int32_t> parse(int argc, char* argv[], CLI::App& app) {
	CLI11_PARSE(app, argc, argv);
	// return INT32_MAX;
	return std::nullopt;
}

std::unique_ptr<sender::parser::Options> sender::parser::parse_cli(int argc, char* argv[]) {
	CLI::App app("MsgSender");
	auto options = sender::parser::add_options(app);
	if (parse(argc, argv, app).has_value()) {
		return nullptr;
	}

	return options;
}
