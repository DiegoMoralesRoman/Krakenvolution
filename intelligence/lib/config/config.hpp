#pragma once

#include "config/err.hpp"

#include "yaml-cpp/yaml.h"

#include <string>
#include <expected>

namespace core::config {
	using LoadResult = std::expected<YAML::Node, LoadErr>;
	using Config = YAML::Node;

	auto load_config(const std::string& path) noexcept -> const LoadResult;
}
