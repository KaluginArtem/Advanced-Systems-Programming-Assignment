#include "fiber.hpp"
#include <cstdint>
#include <cassert>

Fiber::Fiber(void (*function)(), int priority, std::size_t stack_size, void* data):
_stack_bottom(new char[stack_size]), _stack_top(nullptr), _priority(priority), _data(data) {
    char* sp = _stack_bottom + stack_size; // Set stack pointer to the top of the stack

    uintptr_t sp_val = reinterpret_cast<uintptr_t>(sp);
    sp_val &= ~static_cast<uintptr_t>(0xF); // Align stack pointer to 16-byte boundary
    sp = reinterpret_cast<char*>(sp_val);

    sp -= 128;
    _stack_top = sp;

    _context.rip = reinterpret_cast<void*>(function); // Set instruction pointer to the function
    _context.rsp = _stack_top; // Set stack pointer to the top of the stack
}

Fiber::~Fiber() {
    delete[] _stack_bottom;
}

Context* Fiber::getContext() {
    return &_context;
}

int Fiber::getPriority() const {
    return _priority;
}

void Fiber::setPriority(int priority) {
    _priority = priority;
}

void* Fiber::getData() const {
    return _data;
}

void Fiber::setData(void* data) {
    if(data != nullptr) _data = data;
}