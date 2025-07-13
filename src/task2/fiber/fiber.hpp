#pragma once
#include <cstdint>
#include <cstddef>
#include "../../../context/context.hpp"

class Fiber {
public:

    Fiber(void (*function)(), std::size_t stack_size = 4096); // Constructor to create a fiber with a specific function and stack size.

    ~Fiber(); // Destructor to clean up the fiber's resources.
    
    Context* getContext(); // Returns the context of the fiber, which includes the stack and registers.

private:
    Context _context; // The context of the fiber, which includes the stack and registers.
    char* _stack_bottom; // Pointer to the stack memory allocated for the fiber.
    char* _stack_top; // Pointer to the top of the stack memory allocated for the fiber.
};