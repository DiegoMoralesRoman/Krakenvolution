#pragma once

#include "graphics.hpp"
#include <SFML/Graphics/CircleShape.hpp>

namespace core::graphics::impl {
	class Enviroment {
		public:
			Enviroment(Context& ctx);
			void update(Context& ctx);
			void draw(Context& ctx) const;

		private:
			sf::CircleShape circle{50};
	};
}
