#include "channel_linking.hpp"

auto core::extensions::remote::create_subscription(
		const Subscribe &subscription,
		const std::unordered_map<std::string,
		serial::ObserverMapping> &mappings,
		const core::serial::Channel<> &source_channel
	)-> std::optional<rxcpp::composite_subscription> {
	auto topic_name = subscription.topic();

	auto topic_iter = mappings.find(topic_name);

	if (topic_iter != mappings.end()) {
		const auto& [key, topic] = *topic_iter;
		return topic.to_core_channel.rx.subscribe([&source_channel, topic_name](const std::string& msg) {
			RemoteMsg remote_msg;
			Publish* publish = remote_msg.mutable_pub();
			publish->set_topic(topic_name);
			publish->set_msg(msg);
			source_channel.tx.on_next(remote_msg.SerializeAsString());
		});
	}

	return std::nullopt;
}

auto core::extensions::remote::manage_publish(const Publish &msg, const std::unordered_map<std::string, serial::ObserverMapping>& mappings) -> bool {
	auto topic_iter = mappings.find(msg.topic());

	if (topic_iter != mappings.end()) {
		auto& [key, topic] = *topic_iter;

		topic.to_core_channel.tx.on_next(msg.msg());

		return true;
	}

	return false;
}
