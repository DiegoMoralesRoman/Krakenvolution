#pragma once

#include "util/always_false.hpp"
#include <cstdint>
#include <ftxui/component/component.hpp>
#include <google/protobuf/message.h>
#include <type_traits>

namespace sender {
	auto is_for_number(char c) -> bool {
		return 
			  std::isdigit(c)
			|| c == '-'
			|| c == '.';
	}

	auto input_options() -> ftxui::InputOption {
		ftxui::InputOption options;
		options.multiline = false;
		return options;
	}

	template<typename T>
	using arithmetic_input = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, bool>, bool>;
	template<typename T, typename Func>
	auto create_input(
			const Func& on_update,
			const std::string& initial,
			const std::string& placeholder = "", 
			arithmetic_input<T> = true
		) -> ftxui::Component {
		static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type for numeric input.");

		auto data = std::make_shared<std::string>(""); // TODO: add initial value 
		auto input = Input(data.get(), placeholder, input_options());

		input |= ftxui::CatchEvent([=](const ftxui::Event& ev) {
			if (ev.is_character() and not is_for_number(ev.character()[0])) 
				{ return true; }

			if (data->empty()) { return false; }

			try {
				T value = static_cast<T>(std::stod(*data));
				on_update(value);
			} catch (std::invalid_argument exception) {
				return true;
			}
			return false;
		});

		return Renderer(input, [=] {
			return input->Render() | ftxui::border | ftxui::flex;
		});
	}

	template<typename Func>
	auto create_input(const Func& on_update, const std::string& initial, const std::string& placeholder, 
					  std::enable_if_t<std::is_same_v<std::string, std::string>, bool> = true) -> ftxui::Component {
		auto data = std::make_shared<std::string>(initial);
		auto input = Input(data.get(), placeholder, input_options());

		input |= ftxui::CatchEvent([=](const ftxui::Event& ev) {
			on_update(*data);
			return false;
		});

		return Renderer(input, [=] {
			return input->Render() | ftxui::border | ftxui::flex;
		});
	}

	template<typename T>
	auto set_value(
			google::protobuf::Message* msg,
			const google::protobuf::Reflection* reflection,
			const google::protobuf::FieldDescriptor* descriptor,
			const T& value
		) -> void {
		if constexpr (std::is_same_v<T, int32_t>) {
			reflection->SetInt32(msg, descriptor, value);
		} else if constexpr (std::is_same_v<T, int64_t>) {
			reflection->SetInt64(msg, descriptor, value);
		} else if constexpr (std::is_same_v<T, uint32_t>) {
			reflection->SetUInt32(msg, descriptor, value);
		} else if constexpr (std::is_same_v<T, uint64_t>) {
			reflection->SetUInt64(msg, descriptor, value);
		} else if constexpr (std::is_convertible_v<T, std::string>) {
			reflection->SetString(msg, descriptor, value);
		} else if constexpr (std::is_same_v<T, bool>) {
			reflection->SetBool(msg, descriptor, value);
		} else if constexpr (std::is_same_v<T, float>) {
			reflection->SetFloat(msg, descriptor, value);
		} else if constexpr (std::is_same_v<T, double>) {
			reflection->SetDouble(msg, descriptor, value);
		} else {
			static_assert(core::utils::always_false<T>, "There is no available implementation for this type");
		}
	}
}
