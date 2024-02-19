#pragma once
#include <functional>

namespace core {
	template<typename T>
	concept States = requires (T t) {
		requires std::is_enum_v<typename T::State>;
		requires std::is_enum_v<typename T::Event>;

		{ T::State::SIZE } -> std::same_as<typename T::State>;
		{ T::Event::SIZE } -> std::same_as<typename T::Event>;

		{ T::DEFAULT_STATE } -> std::same_as<const typename T::State&>;

		// TODO: optimize this to use an std::tuple for trivially optimizable functions
		{ t.states[0] } -> std::same_as<const std::function<typename T::State(typename T::Event)>&>;
		requires std::is_array_v<decltype(t.states)>;
		requires std::is_const_v<decltype(t.states)>;
		requires std::extent_v<decltype(t.states)> == static_cast<size_t>(T::State::SIZE);

		{ t.transitions[0] } -> std::same_as<const std::function<void()>&>;
		requires std::is_array_v<decltype(t.transitions)>;
		requires std::is_const_v<decltype(t.transitions)>;
		requires std::extent_v<decltype(t.transitions)> == static_cast<size_t>(T::State::SIZE);
	};

	template<States S>
	class StateMachine {
		public:
			StateMachine() {
				this->states.transitions[static_cast<size_t>(this->current_state)]();
			}

			auto update(S::Event ev) -> void {
				auto new_state = this->states.states[static_cast<size_t>(this->current_state)](ev);
				if (new_state != this->current_state) {
					this->states.transitions[static_cast<size_t>(new_state)]();
				}
				this->current_state = new_state;
			}

			auto get_current_state() const -> S::State { return this->current_state; }
			auto get_ctx() const -> const S& { return this->states; };
		private:
			S states {};
			S::State current_state = S::DEFAULT_STATE;
	};
}
