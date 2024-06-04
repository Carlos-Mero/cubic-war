#include "app.h"

#include <SDL.h>
#include <chipmunk.h>
#include <time.h>

#include "player.h"
#include "bullets.h"

// global objects
App app = {
    .running = true,
    .window = NULL,
    .rr = NULL,
    .screen = NULL,
    .window_size = {0, 0, 800, 480},
    .DPI_SCALE = 0.0,
    .rtime = {.tv_sec = 0, .tv_nsec = 0},

    .space = NULL,

    .player = NULL,
};

static const SDL_Color main_bkg_color = {0xbd, 0xd0, 0xf1, 0xff};

int app_init() {
    // initialize SDL lib
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
        printf("SDL2 failed to initialize: %s\n", SDL_GetError());
        goto ON_FAIL;
    }
    app.window = SDL_CreateWindow("CubicWar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  app.window_size.w,
                                  app.window_size.h,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    app.rr = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(app.rr, SDL_BLENDMODE_BLEND);
    int actual_width;
    SDL_GetRendererOutputSize(app.rr, &actual_width, NULL);
    app.DPI_SCALE = (double)actual_width / (double)app.window_size.w;

    // initialize chipmunk
    app.space = cpSpaceNew();

    // initialize objects
    app.player = player_init();
    bullets_init();

    // initialize timer
    timespec_get(&app.rtime, TIME_UTC);

    // returns the result
    return 0;
ON_FAIL:
    return -1;
}

static void render_callback(cpBody* body, void* data) {
    auto obj = (object*)(cpBodyGetUserData(body));
    if (obj->_render) {
        obj->_render(obj);
    }
}

static void main_render_process() {
    SDL_SetRenderTarget(app.rr, NULL);
    SDL_SetRenderDrawColor(app.rr,
                           main_bkg_color.r,
                           main_bkg_color.g,
                           main_bkg_color.b,
                           main_bkg_color.a);
    SDL_RenderClear(app.rr);

    // render subsequent objects
    cpSpaceEachBody(app.space, render_callback, NULL);

    // Final render present
    SDL_RenderPresent(app.rr);
}

static void process_callback(cpBody* body, void* delta) {
    auto obj = (object*)(cpBodyGetUserData(body));
    if (obj->_process) {
        obj->_process(obj, *(double*)delta);
    }
}

static void input_handling() {
    while (SDL_PollEvent(&app.event)) {
        switch(app.event.type) {
            case (SDL_QUIT):
                app.running = false;
                break;
            
            default:
                break;
        }
    }
    return;
}

void main_process() {
    struct timespec ntime;
    timespec_get(&ntime, TIME_UTC);
    double delta = (double)(ntime.tv_sec - app.rtime.tv_sec) + (double)(ntime.tv_nsec - app.rtime.tv_nsec) * 1e-9;
    app.rtime = ntime;

    input_handling();

    // process following objects
    cpSpaceEachBody(app.space, process_callback, &delta);

    // Physical Steps
    cpSpaceStep(app.space, delta);

    // render objects
    main_render_process();
}

static void cleanup_callback(cpBody* body, void* data) {
    auto obj = (object*)(cpBodyGetUserData(body));
    if (obj->_free) {
        cpSpaceAddPostStepCallback(app.space, obj->_free, obj, NULL);
    }
}

void final_cleanup() {
    // collect objects
    cpSpaceEachBody(app.space, cleanup_callback, NULL);

    // deinit globals
    bullets_deinit();

    // cleanup chipmunk
    cpSpaceFree(app.space);
    
    // exit SDL
    SDL_DestroyTexture(app.screen);
    SDL_DestroyRenderer(app.rr);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
