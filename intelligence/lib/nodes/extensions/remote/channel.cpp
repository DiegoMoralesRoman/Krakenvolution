#include "channel.hpp"

using namespace core::extensions::remote;

bool Channel::operator==(const Channel& other) const {
	return this->UID == other.UID;
}
