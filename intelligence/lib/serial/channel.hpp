#pragma once

#include <cstdint>
#include <rxcpp/rx.hpp>

namespace core::serial {
	struct Channel {
		rxcpp::observable<std::string> rx;
		rxcpp::subscriber<std::string> tx;
		uint64_t UID;

		auto operator==(const Channel& other) const -> bool;
	};

	struct ManagedChannel {
		Channel source_channel;
		Channel core_channel;
		std::string name;
		// RXCPP
		rxcpp::subjects::subject<std::string> to_source;
		rxcpp::subjects::subject<std::string> from_source;
	};

	struct ObserverMapping {
		rxcpp::subjects::subject<std::string> from_source;
		Channel to_core_channel;
	};
}
