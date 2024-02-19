#pragma once
#include <coroutine>
#include <cstdlib>
#include <optional>

template<typename T>
struct Generator {
    struct promise_type {
        std::optional<T> current_value;
        auto yield_value(T value) -> std::suspend_always {
            current_value = value;
            return {};
        }
        auto initial_suspend() -> std::suspend_always { return {}; }
        auto final_suspend() noexcept -> std::suspend_always { return {}; }
        auto get_return_object() -> Generator {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        auto return_void() -> void {}
        auto unhandled_exception() -> void {
            std::exit(1);
        }
    };

    struct iterator {
        std::coroutine_handle<promise_type> coro;
        bool done;

        iterator(std::coroutine_handle<promise_type> coro, bool done) : coro(coro), done(done) {}

        auto operator++() -> iterator& {
            coro.resume();
            done = coro.done();
            return *this;
        }

        auto operator*() const -> const T& {
            return *coro.promise().current_value;
        }

        auto operator==(const iterator& other) const -> bool {
            return done == other.done;
        }

        auto operator!=(const iterator& other) const -> bool {
            return !(*this == other);
        }
    };

    std::coroutine_handle<promise_type> coro;

    Generator(std::coroutine_handle<promise_type> h) : coro(h) {}
    ~Generator() { if (coro) coro.destroy(); }

    iterator begin() {
        if (coro) {
            coro.resume();
            if (coro.done()) return end();
        }
        return iterator{coro, false};
    }

    iterator end() {
        return iterator{coro, true};
    }
};
