#pragma once

#include <rxcpp/rx.hpp>
#include <optional>

#include "messages/remote.pb.h"
#include "serial/channel.hpp"

namespace core::extensions::remote {
	std::optional<rxcpp::composite_subscription> create_subscription(const Subscribe& subscription, const std::unordered_map<std::string, serial::ObserverMapping>& mappings, const serial::Channel& source_channel);
	bool manage_publish(const Publish& msg, const std::unordered_map<std::string, serial::ObserverMapping>& mapping);
}
