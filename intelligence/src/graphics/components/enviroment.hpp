#pragma once

#include "../context.hpp"
#include "grid.hpp"

namespace run::graphics::components {
	class Enviroment {
		public:
			Enviroment(Context& ctx);
			void update(Context& ctx);
			void render(Context& ctx);

		private:
			Grid grid;
	};
}
