#pragma once

#include "serial/channel.hpp"

#include <functional>
#include <optional>
#include <vector>
#include <string>

namespace core::extensions::remote {
	class ConnectionManager {
		public:
		ConnectionManager() = default;

		ConnectionManager(const ConnectionManager &) = delete;
		ConnectionManager(ConnectionManager &&) = default;
		ConnectionManager& operator=(const ConnectionManager &) = delete;
		ConnectionManager& operator=(ConnectionManager &&) = default;

		core::serial::Channel& new_channel(const std::string& name);
		bool remove_channel(const core::serial::Channel& channel);

		template <typename Func>
		void on_new_channel(const Func& callback) { this->new_channel_callbacks.push_back(callback); }
		template <typename Func>
		void on_removed_channel(const Func& callback) { this->deleted_channel_callbacks.push_back(callback); }

		// std::optional<std::reference_wrapper<const ManagedChannel>> find_managed_channel(const core::serial::Channel& channel) const;
		std::optional<std::string> find_channel_name(const core::serial::Channel& channel) const;
		std::optional<std::string> find_channel_name(const decltype(core::serial::Channel::UID)& UID) const;

		std::vector<core::serial::ManagedChannel>& channels();
	private:
		std::vector<core::serial::ManagedChannel> connected_channels;

		std::vector<std::function<void(const core::serial::Channel&)>> new_channel_callbacks;
		std::vector<std::function<void(const core::serial::Channel&)>> deleted_channel_callbacks;

		void run_new_channel_callbacks(const core::serial::Channel& channel) const;
		void run_remove_channel_callbacks(const core::serial::Channel& channel) const;
	};
}
