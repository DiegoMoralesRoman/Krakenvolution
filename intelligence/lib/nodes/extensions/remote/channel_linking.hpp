#pragma once

#include <rxcpp/rx.hpp>
#include <optional>

#include <serial/channel.hpp>

#include <messages/remote.pb.h>

namespace core::extensions::remote {
	auto create_subscription(
			const Subscribe& subscription,
			const std::unordered_map<std::string,
			serial::ObserverMapping>& mappings,
			const serial::Channel<>& source_channel
		) -> std::optional<rxcpp::composite_subscription>;

	auto manage_publish(
			const Publish& msg,
			const std::unordered_map<std::string,
			serial::ObserverMapping>& mapping
		) -> bool;
}
