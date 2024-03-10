#pragma once

#include "err.hpp"

#include <serial/channel.hpp>

#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <atomic>
#include <expected>

namespace sender::net {
	class Events {
		public:
			using OnDisconnectCb = std::function<void()>;

			auto on_disconnected(const OnDisconnectCb& cb) -> Events&;

			auto notify_on_disconnected() const -> void;
		private:
			std::vector<OnDisconnectCb> _on_disconnected;
	};

	struct Connection {
		std::thread conn_thread;
		core::serial::Channel<> channel;
		std::function<void()> close;
	};

	using ConnectionResult = std::expected<Connection, ConnectionError>;

	auto connect(
			const Events& ev,
			std::string ip,
			uint16_t port
		) -> ConnectionResult;
};
