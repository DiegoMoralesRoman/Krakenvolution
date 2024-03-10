#include "err.hpp"
#include <format>

auto core::net::err_repr(const ConnectionError &err) -> std::string {
	struct {
		std::string operator()(const core::net::SocketCreateError& err) {
			return std::format("Failed to create socket ({})", err.reason);
		}

		std::string operator()(const core::net::InvalidServerAddrError& err) {
			return std::format("Invalid server address {}", err.addr);
		}

		std::string operator()(const core::net::SocketConnectionError& err) {
			return std::format("Failed to connect ({})", err.reason);
		}
	} visitor;

	return std::visit(visitor, err);
}
