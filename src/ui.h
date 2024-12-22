#pragma once
#ifndef UI_H
#define UI_H

#include <raylib.h>

#include "./include/salvager_internal.h"
#include "./include/types.h"

void ResourceDraw(void);
void ProgressDraw(Rectangle size, f32 current, f32 max, Color active, Color deactive);

void LevelupSelectionDraw(void);
void LevelupSelectionUpdate(void);

void PauseSelectUpdate(void);

void PauseSelectDraw(void);

#endif
