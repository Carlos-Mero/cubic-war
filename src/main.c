#include <stdio.h>
#include <omp.h>
#include <SDL.h>
#include <chipmunk.h>

#include "utils.h"

void calc_test() {
    int sum = 0;
    #pragma omp parallel for
    for (int i = 0; i < 1000; i++) {
        sum++;
    }
    printf("The final sum is: %d\n", sum);
    printf("Which is expected to be: %d\n", 1000);
}

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
void main_loop(void* arg) {
    if (app.running) {
        main_loop();
    }
}
#else
void main_loop() {
    while (app.running) {
        main_process();
    }
}
#endif

int main(int argc, char** argv) {

    if (init_sdllib() < 0) {goto END_CLEANUP;}

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, nullptr, -1, 1)
    #else
    main_loop();
    #endif

END_CLEANUP:
    cleanup_globals();
    return 0;
}
