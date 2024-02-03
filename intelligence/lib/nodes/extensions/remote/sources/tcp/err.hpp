#pragma once

#include <netinet/in.h>
#include <variant>
#include <string>

namespace core::extensions::remote::sources::tcp {
	struct InvalidAddrErr { std::string addr; };
	struct FailedToBindErr { 
		std::string addr_name;
		int port;
		std::string reason;
	};
	struct FailedToListenErr {
		std::string reason;
	};

	struct CantSetReuseAddrErr {
		std::string reason;
	};

	using CreateServerErr = std::variant<
		InvalidAddrErr,
		FailedToBindErr,
		FailedToListenErr,
		CantSetReuseAddrErr
	>;
}
