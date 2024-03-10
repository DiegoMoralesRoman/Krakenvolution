#pragma once

#include <rxcpp/rx.hpp>

#include <cstdint>
#include <string>

namespace core::serial {
	template<typename Signal = std::string>
	struct Channel {
		rxcpp::observable<Signal> rx;
		rxcpp::subscriber<Signal> tx;
		uint64_t UID;

		auto operator==(const Channel<Signal>& other) const -> bool {
			return this->UID == other.UID;
		}
	};

	template<typename Signal = std::string>
	struct ManagedChannel {
		Channel<Signal> source_channel;
		Channel<Signal> core_channel;
		std::string name;
		// RXCPP
		rxcpp::subjects::subject<Signal> to_source;
		rxcpp::subjects::subject<Signal> from_source;
	};

	struct ObserverMapping { // Mappings will always be using string (boundary)
		rxcpp::subjects::subject<std::string> from_source;
		Channel<> to_core_channel;
	};
}
