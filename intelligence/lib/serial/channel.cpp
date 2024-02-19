#include "channel.hpp"

using namespace core::serial;

auto Channel::operator==(const Channel& other) const -> bool {
	return this->UID == other.UID;
}
