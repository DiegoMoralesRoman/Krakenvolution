#include "state_machines/state_machine.hpp"
#include <functional>
#include <gtest/gtest.h>

struct States {
	enum class State {
		A,
		B,
		SIZE
	};
	const static State DEFAULT_STATE = State::A;

	enum class Event {
		INCREMENT,
		DECREMENT,
		SIZE
	};

	int value = 0;
	int trans = 0;

	State state_a(Event ev) {
		switch (ev) {
			case Event::INCREMENT:
				value += 1;
				break;
			case Event::DECREMENT:
				value -= 1;
				break;
			default:
				break;
		}
		return State::B;
	}

	State state_b(Event ev) {
		switch (ev) {
			case Event::INCREMENT:
				value += 2;
				break;
			case Event::DECREMENT:
				value -= 2;
				break;
			default:
				break;
		}
		return State::A;
	}

	void enter_state_a() {
		trans += 1;
	}

	void enter_state_b() {
		trans += 2;
	}

	const std::function<State(Event)> states[2] = {
		std::bind_front(&States::state_a, this),
		std::bind_front(&States::state_b, this)
	};

	const std::function<void()> transitions[2] = {
		std::bind_front(&States::enter_state_a, this),
		std::bind_front(&States::enter_state_b, this)
	};
};

TEST(StateMachine, Basic) {
	core::StateMachine<States> machine;
	const States& states = machine.get_ctx();

	ASSERT_EQ(machine.get_current_state(), States::State::A) << "Default state not set properly";
	ASSERT_EQ(states.trans, 1) << "Default state transition should be called upon creation";

	machine.update(States::Event::INCREMENT);
	ASSERT_EQ(states.value, 1) << "Should've called the state A function";
	machine.update(States::Event::INCREMENT);
	ASSERT_EQ(states.value, 3) << "Should've called the state B function";

	machine.update(States::Event::DECREMENT);
	ASSERT_EQ(states.value, 2) << "Event should've been sent properly";
	machine.update(States::Event::DECREMENT);
	ASSERT_EQ(states.value, 0) << "Event should've been sent properly";
}
