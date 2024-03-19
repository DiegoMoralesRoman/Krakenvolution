#pragma once

#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include <ctime>
#include <ftxui/component/component_base.hpp>
#include <rxcpp/rx.hpp>

#include <vector>
#include <iomanip>
#include <ranges>
#include <chrono>
#include <format>

namespace sender::tui::components {
	struct LogEntry {
		std::string msg;
		enum Type {
			IN_MSG,
			OUT_MSG,
			INFO,
			ERROR,
			DEBUG
		} type;
		decltype(std::chrono::system_clock::now()) datetime = std::chrono::system_clock::now();
	};

	auto log_entry(const LogEntry& entry) -> ftxui::Element {
		using namespace ftxui;

		std::string datetime = std::format("[{:%H:%M:%S}]", std::chrono::time_point_cast<std::chrono::seconds>(entry.datetime));

		Element prefix;
		switch (entry.type) {
			case LogEntry::IN_MSG:
				prefix = text(datetime + " 󰍧  ") | color(Color::Green);
				break;
			case LogEntry::OUT_MSG:
				prefix = text(datetime + " 󰍡  ") | color(Color::Green) | dim;
				break;
			case LogEntry::INFO:
				prefix = text(datetime + " 󰋼  ") | color(Color::Blue);
				break;
			case LogEntry::DEBUG:
				prefix = text(datetime + " 󰃤  ") | color(Color::Yellow);
				break;
			case LogEntry::ERROR:
				prefix = text(datetime + " ❌ ") | color(Color::Red);
		}

		return hbox({
			prefix,
			hflow(paragraph(entry.msg))
		});
	}

	auto log_output(const std::vector<LogEntry>& entries) -> ftxui::Component {
		using namespace ftxui;

		return Renderer([&] {
			std::vector<Element> elements;
			for (const auto& entry : entries | std::views::reverse) {
				elements.emplace_back(log_entry(entry) | xflex);
			}

			if (elements.size() != 0) {
				elements.back() |= focus;
			}


			FlexboxConfig flex_config;
			flex_config.direction = FlexboxConfig::Direction::ColumnInversed;
			flex_config.wrap = FlexboxConfig::Wrap::NoWrap;

			return yframe(flexbox(elements, flex_config));
		});
	}
}
