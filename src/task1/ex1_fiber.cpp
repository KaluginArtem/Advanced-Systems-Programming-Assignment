#include <cstdio>
#include <cstdint>
#include "../../context/context.hpp"

static Context c;
static Context main_context;

void foo() {
    printf("You called foo!\n");

    swap_context(&c, &main_context);
}

int main() {
    const size_t stack_size = 4096; // 4 KiB
    char *stack = new char[stack_size];

    void* sp = stack + stack_size; // Stack grows downwards

    uintptr_t sp_val = reinterpret_cast<uintptr_t>(sp);
    sp_val &= ~static_cast<uintptr_t>(0xF); // Align to 16 bytes
    sp = reinterpret_cast<void*>(sp_val);

    sp = static_cast<char*>(sp) - 128; // Reserve space for local variables

    c.rip = reinterpret_cast<void*>(foo);
    c.rsp = sp;

    swap_context(&main_context, &c);

    printf("Main function resumed after foo() call.\n");
    delete[] stack;
    return 0;
}