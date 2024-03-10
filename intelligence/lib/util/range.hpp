#pragma once

#include <ranges>
#include <utility>

namespace core::utils {
	template<typename T, typename ValueType>
	concept TypedRange = std::ranges::range<T> && std::same_as<std::ranges::range_value_t<T>, ValueType>;

	template<template <class, class...> typename Container, std::ranges::range Range>
	auto from_range(const Range& range) {
		using T = decltype(*std::declval<Range>().begin());
		return Container<T>(range.begin(), range.end());
	}
}
