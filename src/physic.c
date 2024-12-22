#include "physic.h"
#include <raymath.h>

void PushCircleObject(PhysicCircleObject *a, PhysicCircleObject *b)
{
    Vector2 delta = Vector2Subtract(b->position, a->position);
    f32 dist = Vector2Length(delta);
    Vector2 collisionNormal = Vector2Normalize(delta);
    a->position = Vector2Subtract(a->position, collisionNormal);
    b->position = Vector2Add(b->position, collisionNormal);
    Vector2 push = Vector2Multiply(b->velocity, collisionNormal);
    a->velocity = Vector2Add(a->velocity, Vector2Scale(push, 0.2));
    b->velocity = Vector2Scale(b->velocity, 0.95);
}
