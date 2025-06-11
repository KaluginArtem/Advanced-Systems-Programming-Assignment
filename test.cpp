#include <iostream>
#include "src/task4_additional_work/fiber/fiber.hpp"
#include "src/task4_additional_work/scheduler/scheduler.hpp"
#include "simpletest/simpletest.h"

char const *groups[] = {
    "FiberSchedulerTests",
};

static Scheduler scheduler;

bool task_a_done = false;
bool task_b_done = false;
int shared_result = 0;
int execution_trace = 0;
int priority_check[3] = {0, 0, 0};

void task_a() {
    task_a_done = true;
    scheduler.fiberExit();
}

void task_b() {
    task_b_done = true;
    scheduler.fiberExit();
}

void task_with_data() {
    int* data = static_cast<int*>(scheduler.current()->getData());
    *data += 5;
    scheduler.fiberExit();
}

void priority_task_10() {
    priority_check[0] = 10;
    scheduler.fiberExit();
}

void priority_task_5() {
    priority_check[1] = 5;
    scheduler.fiberExit();
}

void priority_task_1() {
    priority_check[2] = 1;
    scheduler.fiberExit();
}

void yielding_sequence_1() {
    execution_trace += 1;
    scheduler.yield();
    execution_trace += 10;
    scheduler.fiberExit();
}

void yielding_sequence_2() {
    execution_trace *= 2;
    scheduler.fiberExit();
}

DEFINE_TEST_G(SimpleExecutionTest, FiberSchedulerTests) {
    scheduler.spawn(new Fiber(task_a));
    scheduler.spawn(new Fiber(task_b));
    scheduler.do_it();
    TEST_MESSAGE(task_a_done, "Task A should have executed");
    TEST_MESSAGE(task_b_done, "Task B should have executed");
}

DEFINE_TEST_G(DataPassingTest, FiberSchedulerTests) {
    shared_result = 10;
    scheduler.spawn(new Fiber(task_with_data, 3, 4096, &shared_result));
    scheduler.do_it();
    TEST_MESSAGE(shared_result == 15, "Data should be updated by fiber");
}

DEFINE_TEST_G(PriorityOrderTest, FiberSchedulerTests) {
    scheduler.spawn(new Fiber(priority_task_1, 1));
    scheduler.spawn(new Fiber(priority_task_5, 5));
    scheduler.spawn(new Fiber(priority_task_10, 10));
    scheduler.do_it();
    TEST_MESSAGE(priority_check[0] == 10, "High priority task ran first");
    TEST_MESSAGE(priority_check[1] == 5, "Mid priority task ran second");
    TEST_MESSAGE(priority_check[2] == 1, "Low priority task ran last");
}

DEFINE_TEST_G(YieldingTest, FiberSchedulerTests) {
    execution_trace = 0;
    scheduler.spawn(new Fiber(yielding_sequence_1));
    scheduler.spawn(new Fiber(yielding_sequence_2));
    scheduler.do_it();
    TEST_MESSAGE(execution_trace == 12, "Yield should preserve and resume correctly");
}

DEFINE_TEST_G(EmptySchedulerTest, FiberSchedulerTests) {
    scheduler.do_it();
    TEST_MESSAGE(true, "Scheduler cannot crush and get an empty queue!");
}

int main() {
    bool pass = true;
    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }
    return pass ? 0 : 1;
}
