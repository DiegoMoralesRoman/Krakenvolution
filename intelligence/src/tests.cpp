#include "rxcpp/operators/rx-observe_on.hpp"
#include "rxcpp/rx-observable.hpp"
#include "rxcpp/rx-subscriber.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
	auto obs = rxcpp::observable<>::interval(std::chrono::milliseconds(500));
}
