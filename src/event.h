#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "./include/types.h"

typedef enum
{
    LEVEL_UP_COLLECTION_RANGE,
    LEVEL_UP_FAST_FIRE,
    LEVEL_UP_MULTIPLY_DROP_RATE,
} LevelType;

void EventUpdate(void);
f32 LevelUpRequirement(void);
void LevelUpPlayer(LevelType);


#endif
