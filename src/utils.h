#pragma once

#ifndef SALVAGER_UTILS_H
#define SALVAGER_UTILS_H

#include <raylib.h>
#include <stdbool.h>
#include "./include/types.h"

bool CheckIfInGameSpace(Rectangle, Camera2D, f32 screenWidth, f32 screenHeight);

#endif
