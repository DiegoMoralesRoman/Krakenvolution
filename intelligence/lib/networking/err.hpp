#pragma once

#include <string>
#include <thread>
#include <variant>

namespace core::net {
	struct SocketCreateError {
		std::string reason;
	};

	struct InvalidServerAddrError {
		std::string addr;
	};
	
	struct SocketConnectionError {
		std::string reason;
		std::string addr;
		uint16_t port;
	};

	using ConnectionError = std::variant<
		SocketCreateError,
		InvalidServerAddrError,
		SocketConnectionError
	>;

	auto err_repr(const ConnectionError& err) -> std::string;
}
