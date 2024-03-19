#include <nodes/extensions/remote/sources/uart/err.hpp>

std::string core::extensions::remote::sources::uart::err_repr(const BindPortErr &err) {
	struct {
		std::string operator()(const CouldNotOpenPort& err) {
			return std::format("{}@{} {}", err.file, err.baudrate, err.reason);
		}

		std::string operator()(const InvalidBaudrateErr& err) {
			return std::format("Invalid baudrate {}", err.baudrate);
		}
	} visitor;
	return std::visit(visitor, err);
}
