#pragma once

#include <SDL.h>
#include <chipmunk.h>

typedef struct Player {
    cpVect pos;
    cpVect speed;
    cpVect acceleration_strenth;
    float weapon_timer;

    SDL_Texture* texture;
    void (*weapon_attack)();
} Player;

extern const int player_init_size;
extern const SDL_Color player_color;
extern const float player_max_speed;
extern const float player_acceleration;
extern const float damp_coefficient;
extern const float default_weapon_colddown;

Player* player_init();
void player_collect(Player* player);
void _process_player(Player* player, double delta);
void _render_player(Player* player);

void default_weapon_attack();
