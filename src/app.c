#include "app.h"

#include <SDL.h>
#include <chipmunk.h>

#include "player.h"

// global objects
App app = {
    .running = true,
    .window = nullptr,
    .rr = nullptr,
    .screen = nullptr,
    .window_size = {0, 0, 800, 480},
    .DPI_SCALE = 0.0f,
    .rtime = 0,

    .space = nullptr,

    .player = nullptr,
};

const SDL_Color main_bkg_color = {0xbd, 0xd0, 0xf1, 0xff};

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
    int logical_width;
    SDL_GetWindowSize(app.window, &logical_width, nullptr);
    app.DPI_SCALE = (double)app.window_size.w / (double)logical_width;

    // initialize chipmunk
    app.space = cpSpaceNew();

    // initialize objects
    app.player = player_init();

    // initialize timer
    app.rtime = SDL_GetPerformanceCounter();

    // returns the result
    return 0;
ON_FAIL:
    return -1;
}

void main_process() {
    uint64_t ntime = SDL_GetPerformanceCounter();
    double delta = (double)(ntime - app.rtime) / (double)SDL_GetPerformanceFrequency();
    app.rtime = ntime;

    while (SDL_PollEvent(&app.event)) {
        switch(app.event.type) {
            case (SDL_QUIT):
                app.running = false;
                break;
            
            default:
                break;
        }
    }

    // process following objects
    _process_player(app.player, delta);

    // render objects
    main_render_process();
}

void main_render_process() {
    SDL_SetRenderTarget(app.rr, NULL);
    SDL_SetRenderDrawColor(app.rr,
                           main_bkg_color.r,
                           main_bkg_color.g,
                           main_bkg_color.b,
                           main_bkg_color.a);
    SDL_RenderClear(app.rr);

    // render subsequent objects
    _render_player(app.player);

    // Final render present
    SDL_RenderPresent(app.rr);
}

void final_cleanup() {
    // cleanup chipmunk
    cpSpaceFree(app.space);

    // collect objects
    player_collect(app.player);
    
    // exit SDL
    SDL_DestroyTexture(app.screen);
    SDL_DestroyRenderer(app.rr);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
