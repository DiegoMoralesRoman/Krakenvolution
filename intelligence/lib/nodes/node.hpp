#pragma once

#include "topics.hpp"

#include <functional>
#include <memory>
#include <optional>
namespace core {
	class Node {
		public:
			virtual void setup(GlobalContext& global) = 0;
			virtual void loop(GlobalContext& global) = 0;
			virtual void end(GlobalContext& global) = 0;
			virtual ~Node() {};
	};

	template<typename Ctx>
	class NodeTemplate : public Node {
		public:
			using SetupFunc = std::function<void(GlobalContext&, Ctx&)>;
			using LoopFunc = std::function<void(GlobalContext&, Ctx&)>;
			using EndFunc = std::function<void(GlobalContext&, Ctx&)>;

			NodeTemplate(const SetupFunc&& setup, const EndFunc&& end)
				: _setup(setup) {}

			NodeTemplate (const SetupFunc&& setup, const EndFunc&& end, const LoopFunc&& loop)
				: _setup(setup), _loop(loop) {}

			virtual ~NodeTemplate() = default;

			virtual void setup(GlobalContext& global) override {
				this->_setup(global, this->ctx);
			}

			virtual void end(GlobalContext& global) override {
				this->_end(global, this->ctx);
			}

			virtual void loop(GlobalContext& global) override {
				if (this->_loop.has_value()) {
					this->_loop.value()(global, this->ctx);
				}
			}

		private:
			Ctx ctx;
			SetupFunc _setup;
			EndFunc _end;
			std::optional<LoopFunc> _loop = std::nullopt;
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
