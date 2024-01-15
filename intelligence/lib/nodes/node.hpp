#pragma once

#include "topics.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
namespace core::nodes {
	using GlobalContext = topics::GlobalContext;

	class Node {
		public:
			virtual void setup(GlobalContext& global) = 0;
			virtual void end(GlobalContext& global) = 0;
			virtual ~Node() {};
	};

	struct ApplicationNode {
		std::unique_ptr<Node> node;
		std::optional<size_t> tick_rate_millis = std::nullopt;
	};

	template<typename Ctx>
	class NodeTemplate : public Node {
		public:
			using SetupFunc = std::function<void(GlobalContext&, Ctx&)>;
			using EndFunc = std::function<void(GlobalContext&, Ctx&)>;

			NodeTemplate(const SetupFunc&& setup, const EndFunc&& end)
				: _setup(setup), _end(end) {}

			virtual ~NodeTemplate() = default;

			virtual void setup(GlobalContext& global) override {
				this->_setup(global, this->ctx);
			}

			virtual void end(GlobalContext& global) override {
				this->_end(global, this->ctx);
			}

		private:
			Ctx ctx;
			SetupFunc _setup;
			EndFunc _end;
	};

	template<typename Context, typename... Args>
	std::unique_ptr<Node> create_node(Args... args) {
		static_assert(
				sizeof...(args) >= 1 || sizeof...(args) <= 2,
				"This function should take between 1 and 3 arguments"
			);
		return std::make_unique<NodeTemplate<Context>>(args...);
	};

}
