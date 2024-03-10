#pragma once

#include "connection.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "log_output.hpp"
#include <algorithm>
#include <ftxui/component/component_base.hpp>

#include <messages/remote.pb.h>

#include <functional>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include <format>

namespace sender::tui::components {
	struct SubscriptionsContext {
		std::vector<std::string> subscriptions;
		std::optional<std::function<void()>> on_connected;
		std::optional<std::function<void()>> on_disconnected;
	};

	void subscribe(const std::shared_ptr<ConnectionContext>& ctx, const std::string& topic) {
		if (ctx->connection.has_value() && ctx->connection->has_value()) {
			RemoteMsg msg;
			Subscribe* sub = msg.mutable_sub();
			sub->set_topic(topic);
			ctx->connection->value().channel.tx.on_next(msg.SerializeAsString());
		}
	}

	void unsubscribe(const std::shared_ptr<ConnectionContext>& ctx, const std::string& topic) {
		if (ctx->connection.has_value() && ctx->connection->has_value()) {
			RemoteMsg msg;
			Unsubscribe* unsub = msg.mutable_unsub();
			unsub->set_topic(topic);
			ctx->connection->value().channel.tx.on_next(msg.SerializeAsString());
		}
	}

	template<typename Func>
	auto subscriptions_tab(const std::shared_ptr<SubscriptionsContext>& ctx, const std::shared_ptr<ConnectionContext>& conn_ctx, const Func& push_log) -> ftxui::Component {
		using namespace ftxui;

		ButtonOption button_option;
		button_option.Simple();
		InputOption input_option;
		input_option.multiline = false;

		auto selected_menu_entry = std::make_shared<int>(0);
		auto topic = std::make_shared<std::string>();

		auto subsriptions_menu = Menu(&ctx->subscriptions, selected_menu_entry.get());
		auto subscribe_button = Button("Subscribe", [=]{
				std::string sel_topic = *topic;
				if (sel_topic.empty()) {
					return;
				}

				auto iter = std::find(ctx->subscriptions.begin(), ctx->subscriptions.end(), sel_topic);
				if (iter == ctx->subscriptions.end()) {
					push_log(LogEntry::INFO, std::format("Subscribed to {}", sel_topic));
					ctx->subscriptions.push_back(sel_topic);
					subscribe(conn_ctx, sel_topic);
				} else {
					push_log(LogEntry::INFO, std::format("Already subscribed to {}", sel_topic));
				}
			}, button_option);

		auto topic_input = Input(topic.get(), "Topic", input_option);

		auto unsubscribe_button = Button("Unsubscribe", [=] {
				if (ctx->subscriptions.empty()) {
					return;
				}
				auto& selected = ctx->subscriptions.at(*selected_menu_entry);
				ctx->subscriptions.erase(ctx->subscriptions.begin() + *selected_menu_entry);
				push_log(LogEntry::INFO, std::format("Unsubscribed from {}", selected));
				unsubscribe(conn_ctx, selected);
			}, button_option);

		ctx->on_connected = [=] {
			for (auto& topic : ctx->subscriptions) {
				subscribe(conn_ctx, topic);
			}
		};

		ctx->on_disconnected = [=] {
			ctx->subscriptions.clear();
		};

		auto inputs = Container::Vertical({
			subscribe_button,
			subsriptions_menu,
			unsubscribe_button,
			topic_input
		});

		return Renderer(inputs, [=]{
			return vbox({
				paragraphAlignCenter("Subscriptions") | xflex | bold,
				separator(),
				subsriptions_menu->Render() | yflex,
				separator(),
				unsubscribe_button->Render(),
				subscribe_button->Render(),
				separator(),
				paragraphAlignCenter("Topic"),
				topic_input->Render(),
			}) | flex;
		});
	}
}
