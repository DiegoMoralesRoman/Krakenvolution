#include "tui.hpp"

#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive

#include <easylogging/easylogging++.h>

#include "components/components.hpp"

using namespace sender;

auto tui::run() -> void {
	using namespace ftxui;

	auto app = sender::tui::components::app();

	auto screen = ScreenInteractive::TerminalOutput();
	screen.Loop(app);
}
