#include "util/generator.hpp"
#include <coroutine>
#include <iostream>

Generator<unsigned int> iota(unsigned int n = 0)
{
    while (true)
        co_yield n++;
}

int main() {
	std::cout << "Hello, world!" << std::endl;
	for (auto& val : iota()) {
		std::cout << "Value: " << val << std::endl;
	}
}
