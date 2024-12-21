#pragma once

#ifndef BULLET_H
#define BULLET_H

#include <raylib.h>
#include "include/salvager_internal.h"

void BulletPlayerCreate(Vector2, MovementParams, f32 damage);
void BulletPlayerDraw(void);
void BulletPlayerUpdate(void);
void BulletPlayerCollisionWithAsteroid(void);

#endif
