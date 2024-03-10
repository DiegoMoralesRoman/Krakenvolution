#include "connection_manager.hpp"

#include <easylogging/easylogging++.h>

#include <algorithm>
#include <functional>

auto core::extensions::remote::ConnectionManager::new_channel(const std::string& name) -> core::serial::Channel<>& {
	rxcpp::subjects::subject<std::string> to_source;
	rxcpp::subjects::subject<std::string> from_source;

	uint64_t new_channel_UID = random();
	core::serial::ManagedChannel<>& new_channel = this->connected_channels.emplace_back(core::serial::ManagedChannel<>{
		.source_channel = core::serial::Channel<> {
			.rx = to_source.get_observable(),
			.tx = from_source.get_subscriber(),
			.UID = new_channel_UID
		},
		.core_channel = core::serial::Channel<> {
			.rx = from_source.get_observable(),
			.tx = to_source.get_subscriber(),
			.UID = new_channel_UID
		},
		.name = name,
		.to_source = from_source,
		.from_source = to_source 
	});

	run_new_channel_callbacks(new_channel.core_channel);

	return new_channel.source_channel;
}

auto core::extensions::remote::ConnectionManager::remove_channel(const core::serial::Channel<>& channel) -> bool {
	auto iter = std::find_if(
		this->connected_channels.begin(),
		this->connected_channels.end(),
		[&channel](const core::serial::ManagedChannel<>& cmp) {
			return cmp.source_channel == channel;
		}
	);

	if (iter != this->connected_channels.end()) {
		run_remove_channel_callbacks(iter->core_channel);
		this->connected_channels.erase(iter);
		return true;
	} else {
		LOG(ERROR) << " Tried to remove non-existant channel: " << channel.UID;
		return false;
	}
}

auto core::extensions::remote::ConnectionManager::channels() -> std::vector<core::serial::ManagedChannel<>>& {
	return this->connected_channels;
}

auto core::extensions::remote::ConnectionManager::run_new_channel_callbacks(const core::serial::Channel<>& channel) const -> void {
	for (const auto& callback : this->new_channel_callbacks) {
		callback(channel);
	}
}

auto core::extensions::remote::ConnectionManager::run_remove_channel_callbacks(const core::serial::Channel<>& channel) const -> void {
	for (const auto& callback : this->deleted_channel_callbacks) {
		callback(channel);
	}
}

auto core::extensions::remote::ConnectionManager::find_channel_name(const core::serial::Channel<>& channel) const -> std::optional<std::string> {
	return find_channel_name(channel.UID);
}

auto core::extensions::remote::ConnectionManager::find_channel_name(const decltype(core::serial::Channel<>::UID)& UID) const -> std::optional<std::string> {
	auto iter = std::find_if(
		this->connected_channels.begin(),
		this->connected_channels.end(),
		[UID](const core::serial::ManagedChannel<>& cmp) {
			return cmp.source_channel.UID == UID;
		}
	);
	
	if (iter == this->connected_channels.end()) { return std::nullopt; }
	else { return iter->name; }
}
