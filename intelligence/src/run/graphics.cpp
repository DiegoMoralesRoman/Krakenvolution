#include "run.hpp"
#include <atomic>

#include "graphics/graphics.hpp"
#include <easylogging/easylogging++.h>

std::optional<std::thread> run::init_graphics_thread(std::atomic<bool>& running, core::topics::GlobalContext& global) {
#ifdef HAS_SFML
		LOG(INFO) << "󰵉 Launching GUI";

		// Window event callback function
		const auto on_gui_event = [&running](run::graphics::Event ev) {
			switch (ev) {
				case run::graphics::Event::CLOSE:
				LOG(INFO) << "Closing GUI window";
				running = false;
				running.notify_all();
				break;
			}
		};

		return run::graphics::init_graphics(running, global, on_gui_event);
#else
		LOG(WARNING) << "Application built without SFML support. Remove the --grahpics flag";
		return std::nullopt;
#endif

}
