#pragma once
#include <deque>
#include "../fiber/fiber.hpp"
#include "../../../context/context.hpp"

class Scheduler {
public:
    Scheduler() = default;
    ~Scheduler() = default;

    void spawn(Fiber* f);
    void do_it();
    void fiber_exit();
    void yeild();
    
private:
    std::deque<Fiber*> _fibers;
    Context _context;
    Fiber* _current_fiber; // Pointer to the currently running fiber.
};