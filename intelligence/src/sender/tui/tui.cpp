#include "tui.hpp"

#include "components/app.hpp"

#include <ftxui/component/screen_interactive.hpp>  // for Component, ScreenInteractive
#include <messages/remote.pb.h>

#include <memory>
#include <utility>

auto sender::tui::run(const parser::Options& options) -> std::tuple<StartFn, ExitFn> {
	auto screen = std::shared_ptr<ftxui::ScreenInteractive>(
		new ftxui::ScreenInteractive(ftxui::ScreenInteractive::Fullscreen())
	);

	return {
		[=, &options] { 
			auto refresh = [=] {
				screen->PostEvent(ftxui::Event::Custom);
			};
			screen->Loop(sender::tui::components::app(options, refresh)); 
		},
		[screen] { screen->Exit(); }
	};
}
