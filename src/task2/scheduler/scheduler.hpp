#pragma once
#include <deque>
#include "../fiber/fiber.hpp"
#include "../../../context/context.hpp"

class Scheduler {
public:
    Scheduler() = default; // Default constructor for the Scheduler class.
    ~Scheduler() = default; // Default destructor for the Scheduler class.

    void spawn(Fiber* f); // Adds a fiber to the scheduler's queue.
    void do_it(); // Runs the fibers in the scheduler's queue
    void fiber_exit(); // Exits the currently running fiber and switches to the next one in the queue.
    
private:
    std::deque<Fiber*> _fibers; // Queue to hold fibers that are ready to run.
    Context _context; // Context to save the state of the currently running fiber.
    Fiber* _current_fiber; // Pointer to the currently running fiber.
};