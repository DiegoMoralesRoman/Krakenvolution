#pragma once

#include <networking/connection.hpp>
#include "../utils/protobuf.hpp"
#include "ftxui/dom/elements.hpp"
#include "log_output.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component_base.hpp>
#include <functional>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/json/json.h>
#include <google/protobuf/message.h>
#include <rxcpp/rx.hpp>

#include <messages/remote.pb.h>

#include <memory>
#include <string>
#include <optional>
#include <format>
#include <vector>


namespace sender::tui::components {
	struct ConnectionStatus {
		std::string ip;
		std::string port;
		std::optional<std::string> err_msg;
		enum Status {
			CONNECTED,
			DISCONNECTED
		} status = DISCONNECTED;
	};

	struct ConnectionContext {
		std::string ip;
		uint16_t port;
		ConnectionStatus status;
		std::optional<decltype(core::net::connect({}, {},{}))> connection = std::nullopt;
		std::optional<std::function<void()>> on_connect;
		std::optional<std::function<void()>> on_disconnect;
	};

	auto connection_status(const ConnectionStatus& status) -> ftxui::Element {
		using namespace ftxui;
		auto err_element = text("");

		if (status.err_msg.has_value()) {
			err_element = paragraph(status.err_msg.value()) | center | color(Color::Red);
		} else if (status.status == ConnectionStatus::CONNECTED) {
			err_element = text("CONNECTED") | center | color(Color::Green);
		}

		return vbox({
			hbox({text("IP: "), text(status.ip)}) | center,
			hbox({text("Port: "), text(status.port)}) | center,
			err_element
		});
	}

	template<typename Func, typename Refresh>
	auto handle_connect(const std::shared_ptr<ConnectionContext>& ctx, const Func& push_log, const Refresh& refresh) -> void {
		if (ctx->connection.has_value()) {
			auto& conn = ctx->connection.value();
			if (conn.has_value()) {
				if (ctx->on_connect.has_value()) { ctx->on_disconnect.value()(); }
				conn->close();
				if (conn->conn_thread.joinable()) { conn->conn_thread.join(); }
			}
			ctx->connection.reset();
		} else {
			core::net::Events ev;
			ev.on_disconnected(
				[=]{ if (ctx->connection.has_value()) handle_connect(ctx, push_log, refresh); refresh(); });
			ctx->connection = core::net::connect(ev, ctx->ip, ctx->port);
			if (ctx->connection->has_value()) {
				ctx->connection->value().conn_thread.detach();
				if (ctx->on_connect.has_value()) { ctx->on_connect.value()(); }
				ctx->connection->value().channel.rx.subscribe([=](const std::string& value) {
					RemoteMsg msg;
					if (msg.ParseFromString(value) and msg.has_pub()) {
						const std::string& data = msg.pub().msg();
						google::protobuf::Any any;
						if (any.ParseFromString(data)) {
							push_log(LogEntry::IN_MSG, std::format("{} -> {}", msg.pub().topic(), any.DebugString()));
							refresh();
						}
					}
				});
			}
		}

		if (ctx->connection.has_value()) {
			const auto& connection = ctx->connection.value();
			if (connection.has_value()) {
				push_log(LogEntry::INFO, std::format("Connected to {}:{}", ctx->ip, ctx->port));
				ctx->status.status = ConnectionStatus::CONNECTED;
				ctx->status.err_msg = std::nullopt;
			} else {
				push_log(LogEntry::ERROR, std::format("Failed to connect to {}:{} ({})", ctx->ip, ctx->port, core::net::err_repr(connection.error())));
				ctx->status.status = ConnectionStatus::DISCONNECTED;
				ctx->status.err_msg = core::net::err_repr(connection.error());
			}
		} else {
			push_log(LogEntry::INFO, std::format("Disconnected from {}:{}", ctx->ip, ctx->port));
			ctx->status.status = ConnectionStatus::DISCONNECTED;
			ctx->status.err_msg = "No connection";
		}

		ctx->status.ip = ctx->ip;
		ctx->status.port = std::to_string(ctx->port);

		if (ctx->connection.has_value() and not ctx->connection.value().has_value()) {
			ctx->connection.reset();
		}
	}

	template<typename Func, typename Refresh>
	auto connection_tab(std::shared_ptr<ConnectionContext>& ctx, const Func& push_log, const Refresh& refresh) -> ftxui::Component {
		using namespace ftxui;

		auto ip_data = std::make_shared<std::string>(ctx->ip);
		auto port_data = std::make_shared<std::string>(std::to_string(ctx->port));
		
		InputOption option;
		option.multiline = false;
		option.on_enter = [=]{
			ctx->ip = *ip_data;
			ctx->port = std::stoi(*port_data);
		};

		auto ip = Input(ip_data.get(), "IP", option);
		auto port = Input(port_data.get(), "Port", option);

		auto inputs = Container::Vertical({ ip, port });

		ButtonOption conn_option;
		conn_option.Simple();

		Component connect_button =
			Button("Connect", [=]{
					handle_connect(ctx, push_log, refresh);
			}, conn_option);
		Component disconnect_button =
			Button("Disconnect", [=]{
					handle_connect(ctx, push_log, refresh);
			}, conn_option);

		handle_connect(ctx, push_log, refresh);

		return Container::Vertical({
			Renderer(inputs, [=] {
				return vbox({
					paragraphAlignCenter("IP") | bold,
					ip->Render() | xflex,
					separator(),
					paragraphAlignCenter("PORT") | bold,
					port->Render() | xflex,
					separator(),
					connection_status(ctx->status) | center | flex 
				});
			}) | yflex,
			Renderer(Container::Vertical({inputs, connect_button, disconnect_button}), [=] {
				if (ctx->connection.has_value() && ctx->connection->has_value()) {
					return disconnect_button->Render();
				} else {
					return connect_button->Render();
				}
			})
		});
	};
}
