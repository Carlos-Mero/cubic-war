#include "utils.h"

App app = {
    .running = true,
    .window = nullptr,
    .rr = nullptr,
    .screen = nullptr,
    .window_size = {0, 0, 0, 0},
    .DPI_SCALE = 0.0
};
const SDL_Color main_bkg_color = {0xbd, 0xd0, 0xf1, 0xff};

int init_sdllib() {
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
        printf("SDL2 failed to initialize: %s\n", SDL_GetError());
        goto ON_FAIL;
    }
    app.window = SDL_CreateWindow("CubicWar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    app.rr = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(app.rr, SDL_BLENDMODE_BLEND);
    SDL_GetRendererOutputSize(app.rr, &app.window_size.w, &app.window_size.h);
    int logical_width;
    SDL_GetWindowSize(app.window, &logical_width, nullptr);
    app.DPI_SCALE = (double)app.window_size.w / (double)logical_width;

    return 0;
ON_FAIL:
    return -1;
}

void main_process() {
    while (SDL_PollEvent(&app.event)) {
        switch(app.event.type) {
            case (SDL_QUIT):
                app.running = false;
                break;
            
            default:
                break;
        }
    }
    _render_app();
}

void _render_app() {
    SDL_SetRenderTarget(app.rr, NULL);
    SDL_SetRenderDrawColor(app.rr,
                           main_bkg_color.r,
                           main_bkg_color.g,
                           main_bkg_color.b,
                           main_bkg_color.a);
    SDL_RenderClear(app.rr);

    SDL_RenderPresent(app.rr);
}

void cleanup_globals() {
    SDL_DestroyTexture(app.screen);
    SDL_DestroyRenderer(app.rr);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}
