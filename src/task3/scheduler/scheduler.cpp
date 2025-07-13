#include "scheduler.hpp"
#include "../../../context/context.hpp"
#include <cassert>

void Scheduler::spawn(Fiber* f) { // Adds a fiber to the scheduler's queue.
    assert(f != nullptr);
    _fibers.push_back(f);
}

void Scheduler::do_it() { // Runs the fibers in the scheduler's queue
    get_context(&_context); // Save the current context before running fibers.
    while (!_fibers.empty()) { // While there are fibers to run
        Fiber* f = _fibers.front(); // Get the next fiber to run
        _fibers.pop_front(); // Remove it from the queue
        _current_fiber = f; // Set the current fiber to the one being run

        swap_context(&_context, f->getContext()); // Switch to the fiber's context

        if (_current_fiber) { // If the fiber is still valid after running
            _fibers.push_back(_current_fiber); // Re-add it to the end of the queue
        }
    }    
}

void Scheduler::fiber_exit() { // Exits the currently running fiber and switches to the next one in the queue.
    assert(_current_fiber); 
    Fiber* f = _current_fiber; // Get the currently running fiber
    _current_fiber = nullptr; // Clear the current fiber pointer
    swap_context(f->getContext(), &_context); // Switch back to the saved context
}

void Scheduler::yeild() { // Allows the currently running fiber to yield control back to the scheduler, enabling other fibers to run.
    assert(_current_fiber); // Ensure there is a current fiber to yield
    Fiber* f = _current_fiber; // Get the currently running fiber
    _fibers.push_back(f); // Re-add the current fiber to the end of the queue
    _current_fiber = nullptr; // Clear the current fiber pointer
    swap_context(f->getContext(), &_context); // Switch back to the saved context
    _current_fiber = f; // Set the current fiber to the one that was yielded
}