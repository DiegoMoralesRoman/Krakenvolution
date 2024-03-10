#include "connection.hpp"
#include "easylogging/easylogging++.h"
#include "err.hpp"
#include "rxcpp/subjects/rx-subject.hpp"
#include <arpa/inet.h>
#include <array>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>

auto core::net::Events::on_disconnected(const OnDisconnectCb& cb) -> core::net::Events& {
	this->_on_disconnected.push_back(cb);
	return *this;
}

auto core::net::Events::notify_on_disconnected() const -> void {
	for (auto& cb : this->_on_disconnected) {
		cb();
	}
}

auto client_worker(
		core::serial::Channel<> channel,
		int sock,
		core::net::Events ev
	) -> void;

auto core::net::connect(
		const Events& ev,
		std::string ip,
		uint16_t port
	) -> ConnectionResult {

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return std::unexpected(SocketCreateError{ .reason =  strerror(errno) });
	}

	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
		return std::unexpected(InvalidServerAddrError{ .addr = ip });
	}

	if (connect(sock, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
		return std::unexpected(SocketConnectionError {
				.reason = strerror(errno),
				.addr = ip,
				.port = port
			});
	}

	rxcpp::subjects::subject<std::string> from_sock;
	rxcpp::subjects::subject<std::string> to_sock;

	core::serial::Channel<> from_sock_channel {
		.rx = to_sock.get_observable(),
		.tx = from_sock.get_subscriber(),
		.UID = static_cast<uint64_t>(random())
	};

	return Connection {
		.conn_thread = std::thread(client_worker,
				from_sock_channel,
				sock,
				ev
			),
		.channel = core::serial::Channel<> {
			.rx = from_sock.get_observable(),
			.tx = to_sock.get_subscriber(),
			.UID = static_cast<uint64_t>(random())
		},
		.close = [=]() -> void {
			// Keep lifetimes of the subjects until the connection is out of scope
			auto from_sock_token = from_sock;
			auto to_sock_token = to_sock;

			int error = 0;
			socklen_t len{};
			int retval = getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len);
			if (retval == 0 and error == 0) {
				shutdown(sock, SHUT_RDWR);
			}
		}
	};
}

auto client_worker(
		core::serial::Channel<> channel,
		int sock,
		core::net::Events ev
	) -> void {

	std::array<char, 4096> buffer;

	auto sub = channel.rx.subscribe([sock](const std::string& msg) {
		send(sock, msg.data(), msg.length(), 0);
	});
	
	while (true) {
		int n_recv = recv(sock, buffer.data(), buffer.size(), 0);
		if (n_recv  <= 0) break;
		std::string message(buffer.data(), n_recv);

		channel.tx.on_next(message);
	}

	sub.unsubscribe();
	ev.notify_on_disconnected();
}
