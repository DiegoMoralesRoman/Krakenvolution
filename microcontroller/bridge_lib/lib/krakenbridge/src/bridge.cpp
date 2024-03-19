#include "bridge.hpp"
#include "pb_decode.h"
#include "pb_encode.h"

#include <cstring>
#include <functional>
#include <pb_arduino.h>

bridge::Bridge bridge::Bridge::create(Stream &stream) {
	Bridge bridge;
	bridge.pb_in = as_pb_istream(stream);
	bridge.pb_out = as_pb_ostream(stream);
	bridge.stream = &stream;

	return  bridge;
}

void bridge::Bridge::read_buffer() {
	RemoteMsg remote = RemoteMsg_init_zero;
	pb_decode(&this->pb_in, RemoteMsg_fields, &remote);
	if (remote.which_msg == RemoteMsg_pub_tag) {
		Publish& pub = remote.msg.pub;
		size_t topic_hash = std::hash<std::string>{}(pub.topic);
		pb_istream_t msg_stream = pb_istream_from_buffer(pub.msg.bytes, pub.msg.size);
		for (const Subscription& sub : this->subscriptions) {
			if (sub.topic_hash == topic_hash) {
				sub.callback(msg_stream);
			}
		}
	} else if (remote.which_msg == RemoteMsg_uart_conn_tag) {
		for (const Subscription& sub : this->subscriptions) {
			remote.which_msg = RemoteMsg_sub_tag;
			Subscribe& sub_msg = remote.msg.sub;
			strncpy(sub_msg.topic, sub.topic.c_str(), sizeof(sub_msg.topic));
			pb_encode(&this->pb_out, RemoteMsg_fields, &remote);
		}
	}
}

// TODO: don't use a void*
void bridge::Bridge::publish(const std::string& topic, void* msg, const pb_msgdesc_s* desc) {
	RemoteMsg remote = RemoteMsg_init_zero;
	remote.which_msg = RemoteMsg_pub_tag;
	auto& pub = remote.msg.pub;
	strncpy(pub.topic, topic.c_str(), topic.size());

	pb_ostream_t msg_stream = pb_ostream_from_buffer(pub.msg.bytes, pub.msg.size);
	pb_encode(&msg_stream, desc, pub.msg.bytes);
	pb_encode(&this->pb_out, RemoteMsg_fields, &remote);
}

void bridge::Bridge::unsubscribe(const std::string& topic) {
	Unsubscribe unsub;
	strncpy(unsub.topic, topic.c_str(), sizeof(unsub.topic));
	pb_encode(&this->pb_out, Unsubscribe_fields, &unsub);
}


