#include <cstdio>
#include "fiber/fiber.hpp"
#include "scheduler/scheduler.hpp"
#include <iostream>

static Scheduler scheduler;

void low(){
    int* shared_data = static_cast<int*>(scheduler.current()->getData());
    std::cout << "Low fiber started with shared data: " << *shared_data << std::endl;
    *shared_data += 1; 
    scheduler.fiberExit();
}

void middle(){
    int* shared_data = static_cast<int*>(scheduler.current()->getData());
    std::cout << "Middle fiber started with shared data: " << *shared_data << std::endl;
    *shared_data *= 5; 
    scheduler.fiberExit(); 
}

void high(){
    int* shared_data = static_cast<int*>(scheduler.current()->getData());
    std::cout << "High fiber started with shared data: " << *shared_data << std::endl;
    *shared_data += 10;
    scheduler.fiberExit();
}

void yielding_fiber() {
    int* shared_data = static_cast<int*>(scheduler.current()->getData());
    std::cout << "Yielding fiber first phase, shared data: " << *shared_data << std::endl;
    *shared_data += 100;

    scheduler.yield();  

    std::cout << "Yielding fiber second phase, shared data: " << *shared_data << std::endl;
    *shared_data += 200;

    scheduler.fiberExit();
}

void fiberThrowError() {
    try {
        throw std::runtime_error("An error occurred in the fiber");
    } catch (...) {
        std::cerr << "Exception caught manually in fiber\n";
        scheduler.fiberExit();
    }
}

int main() {
    int shared_data = 0;

    scheduler.onError([](Fiber* f, std::exception_ptr e) {
        try {
            if (e) std::rethrow_exception(e);
        } catch (const std::exception& ex) {
            std::cerr << "Error in fiber with priority " << ex.what() << std::endl;
        }
    });

    scheduler.spawn(new Fiber(low, 1, 4096, &shared_data));
    scheduler.spawn(new Fiber(high, 10, 4096, &shared_data));
    scheduler.spawn(new Fiber(yielding_fiber, 5, 4096, &shared_data));
    scheduler.spawn(new Fiber(middle, 5, 4096, &shared_data));
    scheduler.spawn(new Fiber(fiberThrowError, 8));
    scheduler.do_it();
    std::cout << "Final shared data value: " << shared_data << std::endl;
    return 0;
}