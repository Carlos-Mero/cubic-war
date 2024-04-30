#pragma once

#include <SDL.h>
#include <chipmunk.h>

#include "utils.h"

typedef struct Player {
    object base;

    cpBody* kbody;
    cpShape* coli_shape;
    float weapon_timer;

    SDL_Texture* texture;
    void (*weapon_attack) (cpSpace* space, void* _player, void* data);
} Player;

extern const int player_init_size;
extern const SDL_Color player_color;
extern const float player_max_speed;
extern const float player_acceleration;
extern const float default_weapon_colddown;

Player* player_init();
void _player_free(cpSpace* space, void* _player, void* data);
void _process_player(void* _player, double delta);
void _render_player(void* _player);

void default_weapon_attack(cpSpace* space, void* _player, void* data);
