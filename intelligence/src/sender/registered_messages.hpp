#pragma once

#include "tui/utils/protobuf.hpp"

// Messages
#include <messages/test.pb.h>

namespace user::sender {
	auto get_proto_message() {
		return ::sender::proto::get_proto_messages<
			////////////////////////////////////////////////////////////////////////////////
			// Add here the protobuf messages that you want to be able to send to a topic //
			////////////////////////////////////////////////////////////////////////////////
			Person,
			Led
		>();
	}
}
