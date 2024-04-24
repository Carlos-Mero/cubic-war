#pragma once

#include <SDL.h>

typedef struct App {
    int running;
    SDL_Window* window;
    SDL_Renderer* rr;
    SDL_Texture* screen;
    SDL_Rect window_size;
    double DPI_SCALE;
    SDL_Event event;
} App;

extern App app;
extern const SDL_Color main_bkg_color;

// This functions initializes the globals in this program.
// returns 0 on success, and returns -1 on failure.
int init_sdllib();
void main_process();
void _render_app();
void cleanup_globals();
