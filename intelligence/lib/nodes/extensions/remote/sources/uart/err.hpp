#pragma once

#include <format>
#include <variant>
#include <string>

namespace core::extensions::remote::sources::uart {
	struct InvalidBaudrateErr {
		int baudrate;
	};

	struct CouldNotOpenPort {
		std::string file;
		int baudrate;
		std::string reason;
	}; 

	using BindPortErr = std::variant<
		CouldNotOpenPort,
		InvalidBaudrateErr
	>;

	std::string err_repr(const BindPortErr& err);
}
