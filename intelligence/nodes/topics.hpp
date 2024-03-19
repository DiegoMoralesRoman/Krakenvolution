#pragma once 

#include <rxcpp/rx-subjects.hpp>
#include <serial/channel.hpp>

// Messages
#include <messages/test.pb.h>

namespace user::topics {
	struct Topics {
		Topics();
		// rxcpp::subjects::subject<messages::Detection> detections;
		rxcpp::subjects::subject<Person> person;
		rxcpp::subjects::subject<Led> led;
		std::unordered_map<std::string, core::serial::ObserverMapping> serialized;
	};
}
