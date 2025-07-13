#pragma once
#include <map>
#include <deque>
#include <functional>
#include <exception>
#include "../fiber/fiber.hpp"
#include "../../../context/context.hpp"

class Scheduler {
public:
    Scheduler() = default;
    ~Scheduler() = default;

    void spawn(Fiber* f);
    void yield();
    void do_it();
    void fiberExit();
    void setPriority(Fiber* f, int priority); // Sets the priority of a fiber, used for scheduling purposes.
    Fiber* current() const; // Returns the currently running fiber

    void onError( std::function<void(Fiber*, std::exception_ptr)> handler); // Sets the error handler for fibers
private:
    std::map<int, std::deque<Fiber*>, std::greater<int>> _fibers; // Map of fibers categorized by priority
    Context _context; // Context to save the state of the currently running fiber
    Fiber* _current_fiber = nullptr; // Pointer to the currently running fiber
    std::function<void(Fiber*, std::exception_ptr)> _error_handler; // Error handler for fibers
};
