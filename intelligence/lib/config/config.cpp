#include "config.hpp"
#include <yaml-cpp/node/parse.h>

// This comes from yaml/Yaml.cpp:50

auto core::config::load_config(const std::string& path) noexcept -> const core::config::LoadResult {
	try {

		auto node = YAML::LoadFile(path);
		return node;

	} catch (const YAML::ParserException e) {
		return std::unexpected(config::ParseErr{ .reason = e.what(), .where = e.msg });
	} catch (const YAML::BadFile e) {
		return std::unexpected(config::FileNotFoundErr{ .file = path });
	} catch (YAML::Exception e) {
		return std::unexpected("Spanish Inquisition");
	}
}

