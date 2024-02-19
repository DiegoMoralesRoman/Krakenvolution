#include <chrono>
#include <iostream>
#include <rxcpp/rx.hpp>
#include <thread>

auto main() -> int {
	rxcpp::subjects::subject<int> nums;
	auto other = nums;
	nums.get_observable().subscribe([](const int msg) { std::cout << "[NUMS] " << msg << std::endl; });

	std::cout << "Size: " << sizeof(nums) << std::endl;

	for (int i = 0; i < 10; i++) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		other.get_subscriber().on_next(i);
	}
}
