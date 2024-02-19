#pragma once

#include "../context.hpp"
#include "grid.hpp"

namespace run::graphics::components {
	class Enviroment {
		public:
			Enviroment(Context& ctx);
			auto update(Context& ctx) -> void;
			auto render(Context& ctx) -> void;

		private:
			Grid grid;
	};
}
