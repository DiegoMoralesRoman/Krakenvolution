#include "channel.hpp"

using namespace core::serial;

bool Channel::operator==(const Channel& other) const {
	return this->UID == other.UID;
}
