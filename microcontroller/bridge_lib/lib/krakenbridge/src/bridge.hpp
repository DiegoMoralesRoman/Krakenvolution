#pragma once

#include <algorithm>
#include <cassert>
#include <pb.h>

#include "Stream.h"
#include "pb_decode.h"
#include "pb_encode.h"

#include <remote.pb.h>

#include <functional>
#include <map>
#include <utility>
#include <vector>
#include <string>

namespace bridge {
	struct Subscription {
		size_t topic_hash;
		std::string topic;
		std::function<void(pb_istream_t&)> callback;
	};

	class Bridge {
		public:
			static Bridge create(Stream& stream);

			void publish(const std::string& topic, void* msg, const pb_msgdesc_s* desc);
			void unsubscribe(const std::string& topic);

			template<typename T>
			void subscribe(const std::string& topic, void(*callback)(const T& msg), const pb_msgdesc_s* desc) {
				int found_index = -1;
				size_t topic_hash = std::hash<std::string>{}(topic);
				for (const Subscription& sub : this->subscriptions) {
					found_index++;
					if (sub.topic_hash == topic_hash) {
						break;
					}
				}

				if (found_index == -1) {
					this->subscriptions.emplace_back(Subscription {
						.topic_hash = topic_hash,
						.topic = topic,
						.callback = [=](pb_istream_t& stream) -> void {
							T msg;
							pb_decode(&stream, desc, &msg);
							callback(msg);
						}
					});
				}

				RemoteMsg remote;
				Subscribe& sub = remote.msg.sub;
				remote.which_msg = RemoteMsg_sub_tag;
				strncpy(sub.topic, topic.data(), sizeof(sub.topic));
				pb_encode(&this->pb_out, RemoteMsg_fields, &remote);
			}

			void read_buffer();
		private:
			Bridge() = default;
			pb_istream_s pb_in;
			pb_ostream_s pb_out;
			Stream* stream;
			std::vector<Subscription> subscriptions;
	};
}
