#include "components.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include <memory>
#include <string>

using namespace ftxui;

auto text_input() -> Component {
	auto data = std::make_shared<std::string>();
	auto input = Input(data.get(), "Placeholder");

	return Renderer(input, [input, data] {
        return hbox(text("Dingus: "), input->Render());
	});
}

auto sender::tui::components::app() -> Component {
	return Container::Vertical({
		text_input(),
		text_input(),
		text_input()
	});
}
