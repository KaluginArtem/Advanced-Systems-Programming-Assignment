#include <cstdio>
#include "fiber/fiber.hpp"
#include "scheduler/scheduler.hpp"

static Scheduler scheduler;

void func1(){
    printf("Fiber 1 before\n");
    scheduler.yeild(); // Yield to allow func2 to run
    printf("Fiber 1 after\n");
    scheduler.fiber_exit();
}

void func2(){
    printf("Fiber 2 started\n");
    scheduler.fiber_exit(); // Exit the fiber
}

int main() {
    scheduler.spawn(new Fiber(func1));
    scheduler.spawn(new Fiber(func2));

    scheduler.do_it();
    
    return 0;
}