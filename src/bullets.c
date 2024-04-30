#include "bullets.h"

#include <math.h>
#include <SDL.h>
#include <chipmunk.h>

#include "utils.h"
#include "app.h"

extern App app;

void bullets_init() {
    player_default_bullet_init();
}

void bullets_deinit() {
    player_default_bullet_deinit();
}

static const double bullets_max_existance = 2.5;
static SDL_Texture* player_default_bullet_texture = nullptr;
const double player_default_bullet_damage = 12.0;
static const double player_default_bullet_speed = 1350.0;
static const int player_default_bullet_length = 40;
static const int player_default_bullet_width = 8;
static const int player_default_bullet_tail_length = 16;
static const SDL_Color player_default_bullet_color = {0x3b, 0x60, 0xa0, 0xff};

void player_default_bullet_init() {
    player_default_bullet_texture =
        SDL_CreateTexture(app.rr,
                          SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_TARGET,
                          player_default_bullet_length,
                          player_default_bullet_width);
    SDL_SetRenderTarget(app.rr, player_default_bullet_texture);
    SDL_SetTextureBlendMode(player_default_bullet_texture,
                            SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app.rr, 0, 0, 0, 0);
    SDL_RenderClear(app.rr);
    SDL_Vertex vertices[] = {
        {{0, (float)player_default_bullet_width / 2.0}, player_default_bullet_color},
        {{player_default_bullet_tail_length, player_default_bullet_width}, player_default_bullet_color},
        {{player_default_bullet_length, player_default_bullet_width}, player_default_bullet_color},
        {{player_default_bullet_length, 0}, player_default_bullet_color},
        {{player_default_bullet_tail_length, 0}, player_default_bullet_color}
    };
    int indices[] = {0, 1, 2, 0, 2, 3, 0, 3, 4};
    SDL_RenderGeometry(app.rr,
                       nullptr,
                       vertices,
                       sizeof(vertices) / sizeof(vertices[0]),
                       indices,
                       sizeof(indices) / sizeof(int));
}

void player_default_bullet_deinit() {
    SDL_DestroyTexture(player_default_bullet_texture);
}

PlayerDefaultBullet* player_default_bullet_new(cpVect pos) {
    PlayerDefaultBullet* bullet = malloc(sizeof(PlayerDefaultBullet));
    bullet->base._process = player_default_bullet_process;
    bullet->base._render = player_default_bullet_render;
    bullet->base._free = player_default_bullet_free;
    bullet->existance_duration = 0.0;

    bullet->kbody = cpSpaceAddBody(app.space, cpBodyNewKinematic());
    cpBodySetUserData(bullet->kbody, bullet);
    bullet->coli_shape = cpSpaceAddShape(app.space,
                                         cpBoxShapeNew(bullet->kbody,
                                                       player_default_bullet_length,
                                                       player_default_bullet_width,
                                                       COLLISION_RADIUS));
    cpShapeSetCollisionType(bullet->coli_shape, BULLET_TYPE);
    cpShapeSetFilter(bullet->coli_shape, cpShapeFilterNew(PLAYER_GROUP,
                                                          PLAYER_BULLET_LAYER,
                                                          ENEMY_LAYER));

    int mouse_x; int mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    cpVect speed = cpv(((double)mouse_x * app.DPI_SCALE - pos.x),
                       ((double)mouse_y * app.DPI_SCALE - pos.y));
    cpBodySetPosition(bullet->kbody, pos);
    cpBodySetVelocity(bullet->kbody, cpvmult(cpvnormalize(speed), player_default_bullet_speed));
    cpBodySetAngle(bullet->kbody, atan2(speed.y, speed.x));

    return bullet;
}

void player_default_bullet_process(void* _bullet, double delta) {
    auto bullet = (PlayerDefaultBullet*)_bullet;
    ((PlayerDefaultBullet*)bullet)->existance_duration += delta;
    if (((PlayerDefaultBullet*)bullet)->existance_duration > bullets_max_existance) {
        cpSpaceAddPostStepCallback(app.space, player_default_bullet_free, bullet, nullptr);
    }
}

void player_default_bullet_render(void* _bullet) {
    auto bullet = (PlayerDefaultBullet*)_bullet;
    cpVect pos = cpBodyGetPosition(((PlayerDefaultBullet*)bullet)->kbody);
    SDL_Rect dst = {(int)pos.x, (int)pos.y, player_default_bullet_length, player_default_bullet_width};
    SDL_RenderCopyEx(app.rr,
                     player_default_bullet_texture,
                     NULL,
                     &dst,
                     cpBodyGetAngle(((PlayerDefaultBullet*)bullet)->kbody) * RADIUS_TO_DEGREE,
                     NULL,
                     SDL_FLIP_NONE);
}

void player_default_bullet_free(cpSpace* space, void* _bullet, void* data) {
    auto bullet = (PlayerDefaultBullet*)_bullet;
    cpSpaceRemoveShape(app.space, ((PlayerDefaultBullet*)bullet)->coli_shape);
    cpSpaceRemoveBody(app.space, ((PlayerDefaultBullet*)bullet)->kbody);
    cpShapeFree(((PlayerDefaultBullet*)bullet)->coli_shape);
    cpBodyFree(((PlayerDefaultBullet*)bullet)->kbody);
    free(bullet);
}
