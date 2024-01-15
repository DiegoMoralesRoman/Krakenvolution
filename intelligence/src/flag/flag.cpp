#include "flag.hpp"
#include <algorithm>
#include <array>
#include <bits/ranges_algo.h>
#include <format>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string>
#include <ranges>
#include <vector>

namespace rv = std::ranges::views;
namespace rg = std::ranges;

#include "../prog_info.hpp"

const std::vector<std::string> INFO_LINES {
	"-----------------------",
	"Created by",
	"Laura Morales Román",
	"-----------------------",
	std::format("{}v", run::info::VERSION)
};

const auto FLAG = R"(
█▄▀ █▀█ ▄▀▄ █▄▀ █▀▀ █▄ █ █ █ █▀█ █   █ █ ▀█▀ ▀█▀ █▀█ █▄ █ 
█ █ █▀▄ █▀█ █ █ ██▄ █ ▀█ ▀▄▀ █▄█ █▄▄ █▄█  █  ▄█▄ █▄█ █ ▀█ 
)";

const auto max_length = 56;

std::string center_text(const std::string& text, int width) {
    if (text.length() >= width) {
        return text;  // Text is too wide to center in the given width
    }

    int total_padding = width - text.length();
    int padding_left = total_padding / 2;

    return std::format("{:>{}}", text, padding_left + text.length());
}

int get_terminal_width() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
		return 80; // default value
    }
    return w.ws_col;  // Number of columns (width)
}

std::string run::flag::flag() {
	std::string f { FLAG };

	const auto terminal_width = get_terminal_width();

	const auto transformed_info = INFO_LINES
		| rv::transform([](const auto& line) { return std::string{line}; })
		| rv::transform([=](const auto& line) {
				return center_text(line, max_length);
		  });

	auto join_lines = [=](const auto& iter) {
		return rg::fold_left(transformed_info, std::string{}, 
			[](const auto& prev, const auto& curr) -> std::string {
				return prev + "\n" + curr;
		});
	};

	const auto info = join_lines(transformed_info);

	f.append(info);

	return f;
}
