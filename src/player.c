#include "player.h"
#include "app.h"
#include "utils.h"
#include "bullets.h"

#include <SDL.h>
#include <chipmunk.h>
#include <stdlib.h>
#include <math.h>

const int player_init_size = 48;
const SDL_Color player_color = {0x3b, 0x60, 0xa0, 0xff};
const float player_max_speed = 750.0;
const float player_acceleration = 3650.0;
const float default_weapon_colddown = 0.16;

Player* player_init() {
    Player* player = malloc(sizeof(Player));
    player->_process = _process_player;
    player->_render = _render_player;
    player->_free = _player_free;
    int xx; int yy;
    SDL_GetRendererOutputSize(app.rr, &xx, &yy);
    player->kbody = cpSpaceAddBody(app.space, cpBodyNewKinematic());
    cpBodySetPosition(player->kbody, cpv((float)xx / 7.0, (float)yy / 2.0));
    cpBodySetUserData(player->kbody, player);
    player->coli_shape = cpSpaceAddShape(app.space,
                                         cpBoxShapeNew(player->kbody,
                                                       player_init_size,
                                                       player_init_size,
                                                       COLLISION_RADIUS));
    cpShapeSetCollisionType(player->coli_shape, ENTITY_TYPE);
    cpShapeSetFilter(player->coli_shape, cpShapeFilterNew(PLAYER_GROUP,
                                                          PLAYER_LAYER,
                                                          PLAYER_LAYER));

    player->texture = SDL_CreateTexture(app.rr, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
                                        player_init_size, player_init_size);
    SDL_SetRenderTarget(app.rr, player->texture);
    SDL_SetRenderDrawColor(app.rr,
                           player_color.r,
                           player_color.g,
                           player_color.b,
                           player_color.a);
    SDL_RenderFillRect(app.rr, NULL);

    player->weapon_attack = default_weapon_attack;
    return player;
}

void _player_free(cpSpace* space, void* _player, void* data) {
    Player* player = (Player*)_player;
    SDL_DestroyTexture(player->texture);
    cpSpaceRemoveShape(space, player->coli_shape);
    cpSpaceRemoveBody(space, player->kbody);
    cpShapeFree(player->coli_shape);
    cpBodyFree(player->kbody);
    free(player);
}

void _process_player(void* _player, double delta) {
    Player* player = (Player*)_player;
    player->weapon_timer -= delta;
    const uint8_t* key_states = SDL_GetKeyboardState(NULL);
    cpVect acc = cpv((cpFloat)(key_states[SDL_SCANCODE_D] - key_states[SDL_SCANCODE_A]),
                     (cpFloat)(key_states[SDL_SCANCODE_S] - key_states[SDL_SCANCODE_W]));
    acc = cpvnormalize(acc);
    cpVect pspeed = cpvmult(cpBodyGetVelocity(player->kbody), exp(- delta * CP_PI));
    pspeed = cpvclamp(pspeed, player_max_speed);
    pspeed = cpvadd(pspeed,
                    cpvmult(acc, player_acceleration * delta));
    cpBodySetVelocity(player->kbody, pspeed);
    cpBodySetAngle(player->kbody, atan2(pspeed.x, pspeed.y));

    // attack
    if (SDL_GetMouseState(NULL, NULL) == 1) {
        cpSpaceAddPostStepCallback(app.space, player->weapon_attack, player, NULL);
    }
}

void _render_player(void* _player) {
    Player* player = (Player*)_player;
    double angle = cpBodyGetAngle(player->kbody) * RADIUS_TO_DEGREE;
    cpVect pos = cpBodyGetPosition(player->kbody);
    SDL_Rect dst = {(int)pos.x, (int)pos.y - player_init_size/2, player_init_size, player_init_size};
    SDL_RenderCopyEx(app.rr,
                     player->texture,
                     NULL,
                     &dst,
                     angle,
                     NULL,
                     SDL_FLIP_NONE);
}

void default_weapon_attack(cpSpace* space, void* _player, void* data) {
    auto player = (Player*)_player;
    if (player->weapon_timer <= 0) {
        player->weapon_timer = default_weapon_colddown;
        player_default_bullet_new(cpBodyGetPosition(player->kbody));
    }
}
