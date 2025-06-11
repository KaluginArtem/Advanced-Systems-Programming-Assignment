#pragma once
#include "../../../context/context.hpp"
#include <cstddef>

class Fiber {
public:
    Fiber(void (*function)(), int priority = 0, std::size_t stack_size = 4096, void* data = nullptr);
    ~Fiber();

    Context* getContext();

    int getPriority() const;
    void setPriority(int priority);

    void* getData() const;
    void setData(void* data);
private:
    Context _context;
    char* _stack_bottom; // Pointer to the bottom of the stack memory allocated for the fiber.
    char* _stack_top;    // Pointer to the top of the stack memory allocated for the fiber.
    int _priority; // Priority of the fiber, used for scheduling purposes.
    void* _data;
};