#include "scheduler.hpp"
#include <cassert>
#include <iostream>

void Scheduler::spawn(Fiber* f) {
    assert(f);
    _fibers[f->getPriority()].push_back(f);
}

void Scheduler::yield() {
    assert(_current_fiber);
    Fiber* f = _current_fiber; // Get the currently running fiber
    swap_context(f->getContext(), &_context);  // Switch back to the saved context
}

void Scheduler::do_it() {
    get_context(&_context);  // Save the current context before running fibers.

    while(!_fibers.empty()) { // While there are fibers to run
        auto it = _fibers.begin();
        assert(it != _fibers.end()); // Ensure there is at least one fiber to run
        auto& queue = it->second; 
        Fiber* f = queue.front(); // Get the fiber with the highest priority
        queue.pop_front(); // Remove it from the queue
        if(queue.empty())  _fibers.erase(it); // Remove the queue if it's empty

        _current_fiber = f;

        try {
            swap_context(&_context, _current_fiber->getContext()); // Switch to the fiber's context
        } catch (...) { // Handle any exceptions that occur during context switching
            if(_error_handler) {
                _error_handler(_current_fiber, std::current_exception()); 
            } else {
                std::cerr << "Unhandled exception in fiber: ";
            }
            delete _current_fiber;
            _current_fiber = nullptr; 
            continue;
        }

        if(_current_fiber) {
            _fibers[_current_fiber->getPriority()].push_back(_current_fiber); // Reinsert the fiber if it still exists
        } else {
            delete f; // If the fiber has exited, delete it
        }
    }
}

void Scheduler::fiberExit() {
    assert(_current_fiber);
    Fiber* f = _current_fiber; 
    _current_fiber = nullptr; // Reset the pointer
    swap_context(f->getContext(), &_context); // Switch back to the scheduler context
}

void Scheduler::setPriority(Fiber* f, int priority) { // Sets the priority of a fiber, used for scheduling purposes.
   assert(f); 
   int old_priority = f->getPriority(); // Check the current priority
   if (old_priority == priority) return; // If the priority hasn't changed, do nothing

   auto it = _fibers.find(old_priority); // Find the fiber's current priority queue
   if (it != _fibers.end()) {
       auto& queue = it->second; 
       for (auto iter = queue.begin(); iter != queue.end(); ++iter) { // Iterate through the fibers in the queue
              if (*iter == f) {
                queue.erase(iter); // Remove the fiber from its old priority queue
                if(queue.empty()) _fibers.erase(it); // Remove the queue if it's empty
                break;
              }
       }
   }
    f->setPriority(priority); // Update the fiber's priority
    _fibers[priority].push_back(f); // Reinsert the fiber into the new priority queue
}

Fiber* Scheduler::current() const {
    return _current_fiber;
}

void Scheduler::onError(std::function<void(Fiber*, std::exception_ptr)> handler) { // Sets the error handler for fibers
    _error_handler = std::move(handler); // Set the error handler
}
