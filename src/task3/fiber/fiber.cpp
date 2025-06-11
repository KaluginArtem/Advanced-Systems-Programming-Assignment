#include <cstdint>
#include <cassert>
#include "fiber.hpp"

Fiber::Fiber(void (*function)(), std::size_t stack_size)
    : _stack_bottom(new char[stack_size]), _stack_top(nullptr) {
    char* sp = _stack_bottom + stack_size; // Set the stack pointer to the top of the stack.

    uintptr_t sp_val = reinterpret_cast<uintptr_t>(sp);
    sp_val &= ~static_cast<uintptr_t>(0xF); // Align the stack pointer to a 16-byte boundary.
    sp = reinterpret_cast<char*>(sp_val);

    sp -= 128;
    _stack_top = sp; // Set the stack top pointer.

    _context.rip = reinterpret_cast<void*>(function); // Set the instruction pointer to the function.
    _context.rsp = sp; // Set the stack pointer in the context.
}

Fiber::~Fiber() {
    // Free the stack memory allocated for the fiber.
    delete[] _stack_bottom;
}

Context* Fiber::getContext() {
    return &_context;
}