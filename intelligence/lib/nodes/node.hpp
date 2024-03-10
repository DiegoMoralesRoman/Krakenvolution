#pragma once

#include <config/config.hpp>
#include <topics/topics.hpp>

#include <functional>
#include <memory>
#include <string>

namespace core::nodes {

	class Node {
		public:
	virtual void setup(topics::GlobalContext& global, const config::Config& cfg) = 0;
	virtual void end(topics::GlobalContext& global, const config::Config& cfg) = 0;
			virtual ~Node() {};
	};

	struct ApplicationNode {
		std::unique_ptr<Node> node;
		std::string name;
	};

	template<typename Ctx>
	class NodeTemplate : public Node {
		public:
			using SetupFunc = std::function<void(topics::GlobalContext&, Ctx&, const config::Config&)>;
			using EndFunc = std::function<void(topics::GlobalContext&, Ctx&, const config::Config&)>;

			NodeTemplate(const SetupFunc&& setup, const EndFunc&& end)
				: _setup(setup), _end(end) {}

			virtual ~NodeTemplate() = default;

			virtual void setup(topics::GlobalContext& global, const config::Config& cfg) override {
				this->_setup(global, this->ctx, cfg);
			}

			virtual void end(topics::GlobalContext& global, const config::Config& cfg) override {
				this->_end(global, this->ctx, cfg);
			}

		private:
			Ctx ctx;
			SetupFunc _setup;
			EndFunc _end;
	};

	template<typename Context, typename... Args>
	auto create_node(Args... args) -> std::unique_ptr<Node> {
		static_assert(
				sizeof...(args) >= 1 || sizeof...(args) <= 2,
				"This function should take between 1 and 3 arguments"
			);
		return std::make_unique<NodeTemplate<Context>>(args...);
	};
}
