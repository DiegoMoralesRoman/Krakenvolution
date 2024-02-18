#include "components.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include <memory>
#include <string>

using namespace ftxui;

Component text_input() {
	auto data = std::make_shared<std::string>();
	auto input = Input(data.get(), "Placeholder");

	return Renderer(input, [input, data] {
        return hbox(text("Dingus: "), input->Render());
	});
}

Component sender::tui::components::app() {
	return Container::Vertical({
		text_input(),
		text_input(),
		text_input()
	});
}
