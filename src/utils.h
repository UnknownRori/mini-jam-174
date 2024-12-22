#pragma once

#ifndef SALVAGER_UTILS_H
#define SALVAGER_UTILS_H

#include <raylib.h>
#include "./include/types.h"
#include <stdbool.h>
#include "./include/types.h"

typedef struct
{
    i32 minute;
    i32 seconds;
} Time;

bool CheckIfInGameSpace(Rectangle, Camera2D, f32 screenWidth, f32 screenHeight);
Rectangle GetCameraBounds(Camera2D, f32 screenWidth, f32 screenHeight);

Time CalculateTime(f32);

void CameraShake(Camera2D*, f32* shakeness, f32 recover);

#endif
