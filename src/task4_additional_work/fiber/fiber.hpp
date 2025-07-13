#pragma once
#include "../../../context/context.hpp"
#include <cstddef>

class Fiber {
public:
    Fiber(void (*function)(), int priority = 0, std::size_t stack_size = 4096, void* data = nullptr); 
    // Constructor to create a fiber with a specific function, priority, stack size, and optional shared data.
    ~Fiber(); // Destructor to clean up the fiber's resources.

    Context* getContext();

    int getPriority() const; // Returns the priority of the fiber, used for scheduling purposes.
    void setPriority(int priority); // Sets the priority of the fiber.

    void* getData() const; // Returns the shared data pointer that the fiber can access.
    void setData(void* data); // Sets the shared data pointer for the fiber.
private:
    Context _context;
    char* _stack_bottom; // Pointer to the bottom of the stack memory allocated for the fiber.
    char* _stack_top;    // Pointer to the top of the stack memory allocated for the fiber.
    int _priority; // Priority of the fiber, used for scheduling purposes.
    void* _data; // Pointer to shared data that the fiber can access.
};