#pragma once

#include "ftxui/component/component.hpp"
#include "serial/topics.hpp"
#include "inputs.hpp"
#include "util/range.hpp"

#include <cstdint>
#include <memory>
#include <messages/test.pb.h>
#include <ftxui/component/component_base.hpp>

#include <google/protobuf/message.h>
#include <google/protobuf/util/json_util.h>
#include <string>
#include <functional>
#include <utility>
#include <vector>

namespace sender::proto {
	struct Message {
		std::string name;
		std::function<std::pair<
				ftxui::Component,
				std::function<std::unique_ptr<google::protobuf::Message>()>
			>()
		> component;
	};

	namespace __impl {
		template<typename Func>
		auto create_input_from_type(google::protobuf::FieldDescriptor::Type type, const Func& f, const std::string& initial, const std::string& placeholder) -> ftxui::Component {
			switch (type) {
				case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
					return create_input<double>(f, initial, placeholder);
				case google::protobuf::FieldDescriptor::TYPE_FLOAT:
					return create_input<float>(f, initial, placeholder);
				case google::protobuf::FieldDescriptor::TYPE_INT64:
					return create_input<int64_t>(f, initial, placeholder);
				case google::protobuf::FieldDescriptor::TYPE_UINT64:
					return create_input<uint64_t>(f, initial, placeholder);
				case google::protobuf::FieldDescriptor::TYPE_INT32:
					return create_input<int32_t>(f, initial, placeholder);
				case google::protobuf::FieldDescriptor::TYPE_BOOL:
					// return create_input<bool>(f, initial, placeholder);
				case google::protobuf::FieldDescriptor::TYPE_STRING:
					return create_input<>(f, initial, placeholder);
				case google::protobuf::FieldDescriptor::TYPE_BYTES:
					return create_input<>(f, initial, placeholder);
				case google::protobuf::FieldDescriptor::TYPE_UINT32:
					return create_input<uint32_t>(f, initial, placeholder);
				default:
					return create_input<>(f, initial, placeholder);
				}
    	}

		template<core::serial::ProtoMessage ProtoMsg>
		auto get_proto_message() {
			const auto* desc = ProtoMsg::descriptor();
			// const auto* desc = Person::descriptor();

			std::vector<const google::protobuf::FieldDescriptor*> descriptors;
			for (size_t i = 0; i < desc->field_count(); i++) {
				descriptors.emplace_back(desc->field(i));
			}

			auto component = [=] {
				auto shared_data = std::make_shared<ProtoMsg>();
				const google::protobuf::Reflection* reflection = shared_data->GetReflection();

				std::vector<ftxui::Component> components;
				for (const google::protobuf::FieldDescriptor* desc : descriptors) {
					components.push_back(create_input_from_type(
								desc->type(),
								[=](const auto& val) {
									set_value(
											shared_data.get(),
											reflection,
											desc,
											val
										);
								},
								{}, desc->name()
							));
				}
				auto container = ftxui::Container::Horizontal(components);
				auto comp = ftxui::Renderer(container, [=] {
					return container->Render();
				});


				auto get_message = [shared_data] -> std::unique_ptr<google::protobuf::Message> {
					return std::make_unique<ProtoMsg>(*shared_data);
				};
				
				return std::make_pair(comp, get_message);
			};

			return Message {
				.name = desc->name(),
				.component = component,
			};
		};

	}

	template<core::serial::ProtoMessage... ProtoMsg>
	auto get_proto_messages() -> std::vector<Message> {
		return {
			__impl::get_proto_message<ProtoMsg>()...
		};
	}

	auto repr_proto_message(const google::protobuf::Message* msg) -> std::string {
		std::string json_str;
		auto result = google::protobuf::util::MessageToJsonString(*msg, &json_str);
		if (result.ok()) {
			return json_str;
		} else {
			return "Failed to decode message";
		}
	}
}
