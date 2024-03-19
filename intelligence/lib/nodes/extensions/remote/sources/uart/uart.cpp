#include "uart.hpp"
#include "nodes/extensions/remote/sources/uart/err.hpp"
#include "serial_port.hpp"

#include "easylogging/easylogging++.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cstring>
#include <format>
#include <functional>
#include <memory>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>
#include <termios.h>

#include <messages/remote.pb.h>

auto baurate_mapping(int baurdate) -> std::optional<int> {
	switch (baurdate) {
		case 9600:
			return B9600;
		case 115200:
			return B115200;
		default:
			return std::nullopt;
	}
}

struct SerialPort {
	std::string file;
	int baudrate;
	int fd;
};

auto port_worker(SerialPort port, core::extensions::remote::ConnectionManager& conn_manager, const core::topics::GlobalContext& global) {
	std::string channel_name = std::format("UART {}@{}", port.file, port.baudrate);
	LOG(INFO) << "󰙜 New UART " << port.file << "@" << port.baudrate;
	auto& channel = conn_manager.new_channel(channel_name);

	// Notify serial port of connection
	RemoteMsg conn_remote_msg;
	UARTConnected* uart_conn = conn_remote_msg.mutable_uart_conn();
	uart_conn->set_connected(true);
	std::string serialized_uart_conn_msg = conn_remote_msg.SerializeAsString();
	write(port.fd, serialized_uart_conn_msg.data(), serialized_uart_conn_msg.size());
	fsync(port.fd);

	auto subscription = channel.rx.subscribe([&](const std::string& data) {
		write(port.fd, data.data(), data.size());
		fsync(port.fd);
	});

	std::array<char, 1024> buffer;
	while(global.running) {
		bool fail = false;
		while (global.running) {
			int n_recv = read(port.fd, buffer.data(), buffer.size());
			if (n_recv <= 0) {
				fail = true;
				break;
			};
			std::string message(buffer.data(), n_recv);

			channel.tx.on_next(message);
		}
		if (fail) {
			LOG(WARNING) << "Failed to read from port " << port.file << "@" << port.baudrate;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	conn_manager.remove_channel(channel);
	LOG(INFO) << "󰙜 Closed UART connection " << port.file << "@" << port.baudrate;
	close(port.fd);
}

auto bind_port(
		const std::string& port,
		int numeric_baudrate,
		core::extensions::remote::SourceSharedContext& shared_ctx,
		const core::topics::GlobalContext& global
	) -> core::extensions::remote::sources::uart::BindPortResult {

	std::optional<int> baudrate = baurate_mapping(numeric_baudrate);
	if (not baudrate.has_value()) {
		return std::unexpected(core::extensions::remote::sources::uart::InvalidBaudrateErr {
			.baudrate = numeric_baudrate
		});
	}
	int port_fd = core::extensions::remote::uart::open_serial(port.c_str(), baudrate.value());

	if (port_fd <= 0) {
		return std::unexpected(core::extensions::remote::sources::uart::CouldNotOpenPort{
			.file = port,
			.baudrate = numeric_baudrate,
			.reason = strerror(errno)
		});
	}

	SerialPort serial_port {
		.file = port,
		.baudrate = numeric_baudrate,
		.fd = port_fd
	};

	return std::thread(port_worker, serial_port, std::ref(shared_ctx.conn_manager), std::cref(global));
}

auto core::extensions::remote::sources::uart::init_server(
		topics::GlobalContext &global,
		uart::Context &ctx,
		SourceSharedContext &shared_ctx,
		const config::Config &cfg
	) -> std::optional<std::thread> {

	auto ports_opt = cfg["ports"].IsDefined() ? std::make_optional(cfg["ports"]):std::nullopt;

	if (not ports_opt.has_value() or not ports_opt->IsSequence()) {
		LOG(WARNING) << "❌ No ports where specified in the configuration";
		return std::nullopt;
	}

	auto& ports = ports_opt.value();

	auto port_listeners = std::make_unique<std::vector<BindPortResult>>();

	for (auto port_node : ports) {
		if (not port_node["file"].IsDefined() or not port_node["baud"].IsDefined()) {
			LOG(WARNING) << "❌ Failed to provide information (file or baud) in the configuration";
			continue;
		}

		std::string port = port_node["file"].as<std::string>();
		int baudrate = port_node["baud"].as<int>();

		auto binded = bind_port(port, baudrate, shared_ctx, global);

		if (binded.has_value()) {
			binded->detach();
		} else {
			LOG(ERROR) << "❌ Failed to open serial port: " << err_repr(binded.error());
		}

		port_listeners->emplace_back(std::move(binded));
	}

	LOG(INFO) << "󰙜 Initializing UART";
	return std::thread([l = std::move(port_listeners), &global] {
		global.running.wait(false);
		// for (auto& listener : *l) {
		// 	if (listener.has_value() and listener->joinable()) {
		// 		listener->join();
		// 	}
		// }
	});
}

