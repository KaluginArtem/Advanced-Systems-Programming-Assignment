#include "scheduler.hpp"
#include "../../../context/context.hpp"
#include <cassert>

void Scheduler::spawn(Fiber* f) {
    assert(f != nullptr);
    _fibers.push_back(f);
}

void Scheduler::do_it() {
    get_context(&_context);
    while (!_fibers.empty()) {
        Fiber* f = _fibers.front();
        _fibers.pop_front();
        _current_fiber = f;

        swap_context(&_context, f->getContext());

        if (_current_fiber) {
            _fibers.push_back(_current_fiber);
        }
    }    
}

void Scheduler::fiber_exit() {
    assert(_current_fiber);
    Fiber* f = _current_fiber;
    _current_fiber = nullptr;
    swap_context(f->getContext(), &_context);
}