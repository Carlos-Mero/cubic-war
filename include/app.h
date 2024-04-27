#pragma once

#include <pthread.h>
#include <SDL.h>
#include <chipmunk.h>

#include "player.h"

typedef struct App {
    int running;
    SDL_Window* window;
    SDL_Renderer* rr;
    SDL_Texture* screen;
    SDL_Rect window_size;
    double DPI_SCALE;
    SDL_Event event;
    uint64_t rtime;

    // Chipmunk
    cpSpace* space;

    Player* player;
} App;

extern App app;

// This functions initializes the globals in this program.
// returns 0 on success, and returns -1 on failure.
int app_init();

void main_process();
void main_render_process();
void final_cleanup();

// Constants
extern const SDL_Color main_bkg_color;
