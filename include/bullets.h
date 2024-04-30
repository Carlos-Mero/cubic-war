#pragma once

#include <SDL.h>
#include <chipmunk.h>

#include "utils.h"

extern const double player_default_bullet_damage;

typedef struct PlayerDefaultBullet {
    object base;
    cpBody* kbody;
    cpShape* coli_shape;
    double existance_duration;
} PlayerDefaultBullet;

void player_default_bullet_init();
PlayerDefaultBullet* player_default_bullet_new(cpVect pos);
void player_default_bullet_process(void* _bullet, double delta);
void player_default_bullet_render(void* _bullet);
void player_default_bullet_free(cpSpace* space, void* _bullet, void* data); // Frees all the stuff and remove the body from the space
void player_default_bullet_deinit();

void bullets_init();
void bullets_deinit();
