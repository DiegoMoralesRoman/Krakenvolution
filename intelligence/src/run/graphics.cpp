#include "run.hpp"

#include "graphics/graphics.hpp"

#include <easylogging/easylogging++.h>

auto run::init_graphics_thread(core::topics::GlobalContext& global) -> std::optional<std::thread> {
#ifdef HAS_SFML
		LOG(INFO) << "󰵉 Launching GUI";

		// Window event callback function
		const auto on_gui_event = [&global](run::graphics::Event ev) {
			switch (ev) {
				case run::graphics::Event::CLOSE:
				LOG(INFO) << "Closing GUI window";
				global.running = false;
				global.running.notify_all();
				break;
			}
		};

		graphics::WinDimensions win_size {
			.width = 800,
			.height = 600	
		};

		return run::graphics::init_graphics(win_size, global, on_gui_event);
#else
		LOG(WARNING) << "Application built without SFML support. Remove the --grahpics flag";
		return std::nullopt;
#endif
}
