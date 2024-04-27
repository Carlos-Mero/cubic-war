#include "player.h"
#include "app.h"
#include "utils.h"

#include <SDL.h>
#include <stdlib.h>
#include <math.h>

const int player_init_size = 48;
const SDL_Color player_color = {0x3b, 0x60, 0xa0, 0xff};
const float player_max_speed = 750.0;
const float player_acceleration = 3250.0;
const float damp_coefficient = 3.5;
const float default_weapon_colddown = 0.16;

Player* player_init() {
    Player* player = malloc(sizeof(Player));
    int xx; int yy;
    SDL_GetRendererOutputSize(app.rr, &xx, &yy);
    player->pos = cpv((float)xx / 7.0, (float)yy / 2.0);
    player->speed = cpvzero;
    player->acceleration_strenth = cpvzero;

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

void player_collect(Player* player) {
    SDL_DestroyTexture(player->texture);
    free(player);
}

void _process_player(Player* player, double delta) {
    const uint8_t* key_states = SDL_GetKeyboardState(NULL);
    player->acceleration_strenth = cpv((cpFloat)(key_states[SDL_SCANCODE_D] - key_states[SDL_SCANCODE_A]),
                                       (cpFloat)(key_states[SDL_SCANCODE_S] - key_states[SDL_SCANCODE_W]));
    player->acceleration_strenth = cpvnormalize(player->acceleration_strenth);
    player->speed = cpvmult(player->speed, exp(- delta * damp_coefficient));
    player->speed = cpvadd(player->speed,
                           cpvmult(player->acceleration_strenth, player_acceleration * delta));
    player->speed = cpvclamp(player->speed, player_max_speed);
    
    player->pos = cpvadd(player->pos, cpvmult(player->speed, delta));
    printf("Current speed: %f\n Current delta: %f", cpvlength(player->speed), delta);
}

void _render_player(Player* player) {
    double angle = atan2(player->speed.x, player->speed.y) * RADIUS_TO_DEGREE;
    SDL_Rect dst = {(int)player->pos.x, (int)player->pos.y, player_init_size, player_init_size};
    SDL_RenderCopyEx(app.rr,
                     player->texture,
                     NULL,
                     &dst,
                     angle,
                     NULL,
                     SDL_FLIP_NONE);
}

// TODO
void default_weapon_attack() {

}
