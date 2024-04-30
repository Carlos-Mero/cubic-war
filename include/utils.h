#pragma once

#include <chipmunk.h>

typedef struct object {
    void (*_process) (void* obj, double delta);
    void (*_render) (void* obj);
    void (*_free) (cpSpace* space, void* obj, void* data);
} object;

extern const double RADIUS_TO_DEGREE;
extern const double DEGREE_TO_RADIUS;
extern const double COLLISION_RADIUS;


enum CollisionTypes {
    ENTITY_TYPE,
    BULLET_TYPE,
};

enum CollisionShapeGroups {
    NONE_GROUP = 0,
    PLAYER_GROUP = 1,
    ENEMY_GROUP = 2,
};

enum CategoriesAndMasks {
    PLAYER_LAYER = 0b1,
    ENEMY_LAYER = 0b10,
    PLAYER_BULLET_LAYER = 0b100,
    ENEMY_BULLET_LAYER = 0b1000,
};
