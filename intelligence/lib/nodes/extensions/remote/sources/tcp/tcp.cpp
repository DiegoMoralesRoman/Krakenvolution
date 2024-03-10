#include "tcp.hpp"

#include "context.hpp"

#include <nodes/extensions/remote/sources/tcp/err.hpp>

#include <easylogging/easylogging++.h>

#include <algorithm>
#include <array>
#include <asm-generic/socket.h>
#include <cerrno>
#include <cstring>
#include <format>
#include <functional>
#include <sys/select.h>
#include <thread>
#include <variant>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>

void handle_new_connections(
		core::extensions::remote::sources::tcp::Context& ctx,
		core::topics::GlobalContext& global,
		core::extensions::remote::SourceSharedContext& shared_ctx
	);

auto core::extensions::remote::sources::tcp::create_server(
		topics::GlobalContext &global,
		tcp::Context& ctx,
		SourceSharedContext& shared_ctx,
		const config::Config& cfg
	) -> core::extensions::remote::sources::tcp::CreateServerResult {

	const auto port = cfg["port"].IsDefined() ? cfg["port"].as<int>():8080;
	const auto addr = cfg["addr"].IsDefined() ? cfg["addr"].as<std::string>():"127.0.0.1";
	const auto max_conns = cfg["max_conns"].IsDefined() ? cfg["max_conns"].as<int>():10;

	ctx.sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in c_addr;
	c_addr.sin_family = AF_INET;
	c_addr.sin_port = htons(port);

	int opt = 1;
	if (setsockopt(ctx.sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		return std::unexpected(CantSetReuseAddrErr{ .reason = strerror(errno) });
	}

	// Set IPv4 addr from string
	if (inet_pton(AF_INET, addr.c_str(), &c_addr.sin_addr)<=0) {
		return std::unexpected(InvalidAddrErr{ .addr = addr });
	}

	if (bind(ctx.sock_fd, reinterpret_cast<sockaddr*>(&c_addr), sizeof(c_addr)) < 0) {
		return std::unexpected(FailedToBindErr{ .addr_name = addr, .port = port, .reason = strerror(errno) });
	}

	if (listen(ctx.sock_fd, max_conns) < 0) {
		return std::unexpected(FailedToListenErr{});
	}

	ctx.addr = c_addr;

	LOG(INFO) << " Open to TCP connections at " << addr << ":" << port;
	return std::thread(handle_new_connections, std::ref(ctx), std::ref(global), std::ref(shared_ctx));
}

auto handle_client(
		const core::topics::GlobalContext& global,
		int socket_fd,
		sockaddr_in remote_addr,
		core::extensions::remote::ConnectionManager& conn_manager
	) -> void {

	LOG(INFO) << "󰈁 New connection " << inet_ntoa(remote_addr.sin_addr) << ":" << ntohs(remote_addr.sin_port);

	std::string channel_name = std::format("TCP {}:{}", inet_ntoa(remote_addr.sin_addr), ntohs(remote_addr.sin_port));
	auto& channel = conn_manager.new_channel(channel_name);

	// CORE -> SOURCE Portion of the client
	auto subscription = channel.rx.subscribe([socket_fd, &channel_name](const std::string& data) {
		send(socket_fd, data.data(), data.size(), 0);
	});

	// SOURCE -> CORE Portion of the client
	std::array<char, 1024> buffer;
	while (global.running) {
		int n_recv = recv(socket_fd, buffer.data(), buffer.size(), 0);
		if (n_recv <= 0) break;
		std::string message(buffer.data(), n_recv);

		channel.tx.on_next(message);
	}

	conn_manager.remove_channel(channel);
	LOG(INFO) << "󰈂 Closed connection " << inet_ntoa(remote_addr.sin_addr) << ":" << ntohs(remote_addr.sin_port);
	close(socket_fd);
}

auto handle_new_connections(
		core::extensions::remote::sources::tcp::Context& ctx,
		core::topics::GlobalContext& global,
		core::extensions::remote::SourceSharedContext& shared_ctx
	) -> void {

	fd_set file_descriptors{};
	timeval tv{};

	struct Connection {
		std::thread thread_handle;
		int socket_fd;
	};
	std::vector<Connection> connections;

	int fail_count = 0;
	constexpr int MAX_FAILS = 3;

	fcntl(ctx.sock_fd, F_SETFL, O_NONBLOCK);
	while (global.running) {
		FD_ZERO(&file_descriptors);
		FD_SET(ctx.sock_fd, &file_descriptors);

		tv.tv_usec = 500E3;
		int retval = select(ctx.sock_fd + 1, &file_descriptors, NULL, NULL, &tv);
		if (retval == -1) {
			LOG(ERROR) << " Failed to select socket for accepting connections";
			if (fail_count++ >= MAX_FAILS) {
				LOG(ERROR) << " Too many failures, closing server...";
				break;
			}
		} else if (retval) {
			sockaddr_in in_conn_addr{};
			socklen_t addr_len{};
			int new_socket = accept(ctx.sock_fd, reinterpret_cast<sockaddr*>(&in_conn_addr), &addr_len);

			connections.push_back(Connection{
				.thread_handle = std::thread(handle_client, std::ref(global), new_socket, in_conn_addr, std::ref(shared_ctx.conn_manager)),
				.socket_fd = new_socket
			});
		} else {
			// Timeout here
			connections.erase(std::remove_if(connections.begin(), connections.end(), [](const Connection& conn) { 
					return not conn.thread_handle.joinable(); 
				}), connections.end());
		}
	}
	close(ctx.sock_fd);
	// Close all connections
	LOG(INFO) << "󰔟 Closing all connections";
	for (auto& conn : connections) {
		shutdown(conn.socket_fd, SHUT_RDWR);
		close(conn.socket_fd);
		if (conn.thread_handle.joinable()) { conn.thread_handle.join(); }
	}
	LOG(INFO) << " Connections closed";
}

// ////////////// //
// Initialization //
// ////////////// //

auto core::extensions::remote::sources::tcp::init_server(
		topics::GlobalContext &global,
		tcp::Context& ctx,
		SourceSharedContext& shared_ctx,
		const config::Config& cfg
	) -> std::optional<std::thread> {

	auto result = create_server(global, ctx, shared_ctx, cfg);

	if (not result.has_value()) {
		struct {
			std::string operator()(const InvalidAddrErr& err) {
				return std::format("Invalid listen address: {}", err.addr);
			}
			std::string operator()(const FailedToBindErr& err) {
				return std::format("Failed to bind address with info: {}:{} : {}", err.addr_name, err.port, err.reason);
			}
			std::string operator()(const FailedToListenErr& err) {
				return std::format("Failed to listen on specified address");
			}
			std::string operator()(const CantSetReuseAddrErr& err) {
				return std::format("Failed to set REUSEADDR: {}", err.reason);
			}
		} visitor;
		LOG(ERROR) << " Failed to initialize TCP server:";
		LOG(ERROR) << std::visit(visitor, result.error());
		return std::nullopt;
	}

	return std::move(result.value());
}
