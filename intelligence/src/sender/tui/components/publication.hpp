#pragma once

#include "../utils/protobuf.hpp"
#include "../../registered_messages.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/dom/elements.hpp"
#include "util/range.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>

#include <memory>
#include <optional>
#include <ranges>

// Proto messages
#include <messages/test.pb.h>
#include <string>

namespace sender::tui::components {
	struct PublicationContext {
		decltype(proto::get_proto_messages<>()) messages = proto::get_proto_messages<
			Person
		>();

		std::optional<decltype(messages.begin()->component())> current_component = std::nullopt;
		std::string topic;
	};
	
	auto publication_tab(const std::shared_ptr<PublicationContext>& ctx, ftxui::Component dynamic_menu) -> ftxui::Component {
		using namespace ftxui;

		auto menu_entries = std::make_shared<std::vector<std::string>>(core::utils::from_range<std::vector>(
				ctx->messages |
				std::views::transform([](const proto::Message& msg) { return msg.name; })
			));
		auto selected_entry = std::make_shared<int>(0);

		ftxui::MenuOption option;

		option.on_enter = [=] {
			auto menu_entries_token = menu_entries;
			ctx->current_component = ctx->messages.at(*selected_entry).component();
			dynamic_menu->DetachAllChildren();
			dynamic_menu->Add(ctx->current_component->first);
		};

		auto menu = Menu(menu_entries.get(), selected_entry.get(), option) | yflex;

		InputOption topic_option;
		topic_option.multiline = false;
		auto topic_in = Input(&ctx->topic, "topic", topic_option);

		return Renderer(Container::Vertical({menu, topic_in}), [=] {
			return vbox({
				paragraphAlignCenter("Message type"),
				separator(),
				menu->Render(),
				separator(),
				paragraphAlignCenter("Topic"),
				topic_in->Render()
			});
		}) | flex;
	};
}
