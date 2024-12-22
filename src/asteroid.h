#pragma once

#ifndef ASTEROID_H
#define ASTEROID_H

#include <raylib.h>
#include "./include/salvager_internal.h"
#include "include/timer.h"
#include "./include/types.h"

void CreateNewAsteroid(Vector2 pos, MovementParams move, f32 hp);
void AsteroidDraw(void);
void AsteroidUpdate(void);

void AsteroidCollision(void);

void SetGenerateAsteroidInterval(Timer time);
void GenerateAsteroid(void);

#endif
