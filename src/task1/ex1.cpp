#include <cstdio>
#include "../../context/context.hpp"

volatile int global_var = 0;

int main() {
    Context c;

    printf("This text will not be duplicated in the output.\n");
    
    get_context(&c);

    printf("This text will be duplicated in the output.\n");

    if(global_var == 0) {
        global_var += 1;
        set_context(&c);
    }
    return 0;
}