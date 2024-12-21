#pragma once

#ifndef SCRAP_H
#define SCRAP_H

#include <raylib.h>
#include "./include/types.h"

void CreateNewScrap(Vector2 pos, f32 val);

void ScrapDraw(void);

void ScrapUpdate(void);

void ScrapRemove(u32 id);

void ScrapPickup(void);


#endif
