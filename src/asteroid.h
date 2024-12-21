#pragma once

#ifndef ASTEROID_H
#define ASTEROID_H

#include <raylib.h>
#include "./include/salvager_internal.h"
#include "./include/types.h"

void CreateNewAsteroid(Vector2 pos, MovementParams move);
void AsteroidDraw(void);
void AsteroidUpdate(void);

#endif
