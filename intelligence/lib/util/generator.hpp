#pragma once
#include <coroutine>
#include <cstdlib>
#include <optional>

template<typename T>
struct Generator {
    struct promise_type {
        std::optional<T> current_value;
        std::suspend_always yield_value(T value) {
            current_value = value;
            return {};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        void return_void() {}
        void unhandled_exception() {
            std::exit(1);
        }
    };

    struct iterator {
        std::coroutine_handle<promise_type> coro;
        bool done;

        iterator(std::coroutine_handle<promise_type> coro, bool done) : coro(coro), done(done) {}

        iterator& operator++() {
            coro.resume();
            done = coro.done();
            return *this;
        }

        const T& operator*() const {
            return *coro.promise().current_value;
        }

        bool operator==(const iterator& other) const {
            return done == other.done;
        }

        bool operator!=(const iterator& other) const {
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
