#include <cstdio>
#include "../../context/context.hpp"

volatile int global_var = 0;

int main() {
    Context c;

    printf("This text will not be duplicated in the output.\n");
    
    get_context(&c); // Save the current context

    printf("This text will be duplicated in the output.\n");

    if(global_var == 0) { // Check if this is the first run
        global_var += 1;
        set_context(&c); // Restore the saved context
    }
    return 0;
}