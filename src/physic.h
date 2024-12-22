#pragma once

#ifndef PHYSIC_H
#define PHYSIC_H

#include <raylib.h>
#include "./include/types.h"

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    f32 radius;
} PhysicCircleObject;

void PushCircleObject(PhysicCircleObject *a, PhysicCircleObject *b);

#endif
