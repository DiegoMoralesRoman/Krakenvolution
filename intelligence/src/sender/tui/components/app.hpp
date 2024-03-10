#pragma once

#include "../../parser/parser.hpp"

#include "connection.hpp"
#include "ftxui/dom/elements.hpp"
#include "publication.hpp"
#include "log_output.hpp"
#include "subscription.hpp"

#include <messages/remote.pb.h>

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <google/protobuf/message.h>
#include <rxcpp/rx.hpp>

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <vector>

constexpr size_t MAX_BACKLOG = 100;

namespace sender::tui::components {
	auto app(
			const sender::parser::Options& options,
			std::function<void()> refresh
		) -> ftxui::Component {
		using namespace ftxui;
		
		auto selected_tab = std::make_shared<int>(0);
		std::shared_ptr<std::vector<std::string>> tab_entries(new std::vector<std::string>{
				"Connection",
				"Publish",
				"Subscribe"
			});

		auto publication_ctx = std::make_shared<PublicationContext>(PublicationContext{
			});
		auto subscription_ctx = std::make_shared<SubscriptionsContext>(SubscriptionsContext {
			});
		auto connection_ctx = std::make_shared<ConnectionContext>(ConnectionContext{
				.ip = options.ip,
				.port = options.port,
				.on_connect = [=]{
					if (subscription_ctx->on_connected.has_value())
						{ subscription_ctx->on_connected.value()(); }
				},
				.on_disconnect = [=]{
					if (subscription_ctx->on_disconnected.has_value())
						{ subscription_ctx->on_disconnected.value()(); }
				}
			});


		auto log = std::make_shared<std::vector<LogEntry>>();
		auto push_log = [=](LogEntry::Type type, const std::string& msg) {
			log->emplace_back(LogEntry{
					.msg = msg,
					.type = type
				});

			if (log->size() > MAX_BACKLOG) {
				log->erase(log->begin());
			}
		};

		// Component publication_component = publication_tab(publication_ctx);
		// Component connection_component = connection_tab(connection_ctx, refresh);

		ButtonOption tab_selector_option;
		tab_selector_option.Simple();

		auto tab_names = std::make_shared<std::vector<std::string>>(std::vector<std::string>{
			"   ",
			"   ",
			"   "
		});

		Component dynamic_menu = Container::Vertical({
			Renderer([]{
				return paragraphAlignCenter("No component selected");
			}) | borderDashed
		});

		Component tabs =
				Container::Tab({
					connection_tab(connection_ctx, push_log, refresh),
					publication_tab(publication_ctx, dynamic_menu),
					subscriptions_tab(subscription_ctx, connection_ctx, push_log)
				}, selected_tab.get()) | yflex;

		Component tab_selector = Toggle(tab_names.get(), selected_tab.get()) | xflex;
		tab_selector |= CatchEvent([=](const Event& ev) { auto tab_names_token = tab_names; return false; });

		ButtonOption send_button_option;
		send_button_option.Simple();

		return Container::Horizontal({
			Renderer(Container::Vertical({tab_selector, tabs}), [=] {
				return vbox({
					tab_selector->Render(),
					separator(),
					tabs->Render()
				});
			}) | border,
			Container::Vertical({
				log_output(*log) | flex | border,
				dynamic_menu,
				Container::Horizontal({
					Button("󰍡  Send", [=]{

						if (not connection_ctx->connection.has_value()) {
							push_log(LogEntry::ERROR, "There is no connection available");
							return;
						}

						if (not publication_ctx->current_component.has_value()) {
							push_log(LogEntry::INFO, "There is no message available");
							return;
						}

						if (publication_ctx->topic.empty()) {
							push_log(LogEntry::INFO, "Can't send message to empty topic");
							return;
						}

						auto ui_msg = publication_ctx->current_component.value().second();

						RemoteMsg msg;
						Publish* pub = msg.mutable_pub();
						pub->set_topic(publication_ctx->topic);
						pub->set_msg(ui_msg->SerializeAsString());

						connection_ctx->connection->value().channel.tx.on_next(msg.SerializeAsString());
						push_log(LogEntry::OUT_MSG, 
								std::format("{} <- {}", publication_ctx->topic, proto::repr_proto_message(ui_msg.get()))
							);
					}, send_button_option) | xflex,
					Button("  󰃢  ", [=] {
						log->clear();
						refresh();
					}, send_button_option)
				}),
			}) | flex
		});
	}
}
