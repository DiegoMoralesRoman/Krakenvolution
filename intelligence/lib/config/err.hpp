#pragma once

#include <string>
#include <variant>

namespace core::config {
	struct ParseErr { 
		std::string reason;
		std::string where;
	};

	struct FileNotFoundErr {
		std::string file;
	};

	using LoadErr = std::variant<
		ParseErr,
		FileNotFoundErr,
		std::string
	>;	
}
