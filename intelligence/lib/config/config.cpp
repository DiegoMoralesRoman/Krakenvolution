#include "config.hpp"

// This comes from yaml/Yaml.cpp:50
static const std::string g_ErrorCannotOpenFile = "Cannot open file.";

const core::config::LoadResult core::config::load_config(const std::string& path) noexcept {
	Yaml::Node root;
	try {

		Yaml::Parse(root, path.c_str());

	} catch (const Yaml::Exception e) {
		switch (e.Type()) {
			case Yaml::Exception::InternalError:
				break;
			case Yaml::Exception::ParsingError:
				return std::unexpected(ParseErr{
					.reason = e.what(),
					.where = e.Message()
				});
			case Yaml::Exception::OperationError:
				if (e.what() == g_ErrorCannotOpenFile) {
					return std::unexpected(FileNotFoundErr{ .file = path });
				}
		}
	} 

	return root;
}
