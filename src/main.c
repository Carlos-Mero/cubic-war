#include <SDL.h>
#include <chipmunk.h>

#include "app.h"

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

    if (app_init() < 0) {goto END_CLEANUP;}

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(main_loop, nullptr, -1, 1)
    #else
    main_loop();
    #endif

END_CLEANUP:
    final_cleanup();
    return 0;
}
