#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include "./types.h"

typedef struct {
    f32 remaining;
    f32 lifetime;
    bool repeating;
    bool paused;
    bool completed;
    bool previously_completed;
} Timer;

void TimerReset(Timer *time);
void TimerUpdate(Timer *time);
bool TimerCompleted(const Timer *time);
f32 TimeProgress(const Timer *time);
Timer InitTimer(float lifetime, bool repeating);
bool IsTimerDone(const Timer *time);

#endif
