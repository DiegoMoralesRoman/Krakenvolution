#pragma once

#include "channel.hpp"

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

		Channel& new_channel(const std::string& name);
		bool remove_channel(const Channel& channel);

		template <typename Func>
		void on_new_channel(const Func& callback) { this->new_channel_callbacks.push_back(callback); }
		template <typename Func>
		void on_removed_channel(const Func& callback) { this->deleted_channel_callbacks.push_back(callback); }

		// std::optional<std::reference_wrapper<const ManagedChannel>> find_managed_channel(const Channel& channel) const;
		std::optional<std::string> find_channel_name(const Channel& channel) const;
		std::optional<std::string> find_channel_name(const decltype(Channel::UID)& UID) const;

		std::vector<ManagedChannel>& channels();
	private:
		std::vector<ManagedChannel> connected_channels;

		std::vector<std::function<void(const Channel&)>> new_channel_callbacks;
		std::vector<std::function<void(const Channel&)>> deleted_channel_callbacks;

		void run_new_channel_callbacks(const Channel& channel) const;
		void run_remove_channel_callbacks(const Channel& channel) const;
	};
}
