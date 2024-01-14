#include "flag.hpp"
#include <string>

constexpr auto FLAG = R"(
█▄▀ █▀█ ▄▀▄ █▄▀ █▀▀ █▄ █ █▀▀ 
█ █ █▀▄ █▀█ █ █ ██▄ █ ▀█ ▄██ 
)";

std::string run::flag::flag() {
	return std::string{FLAG};
}
