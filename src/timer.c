#include <raylib.h>
#include "./include/timer.h"

void TimerReset(Timer *time)
{
    time->remaining = time->lifetime;
    time->completed = false;
    time->previously_completed = false;
}

void TimerUpdate(Timer *time)
{
    if (time->paused) return;
    time->previously_completed = time->completed;
    if (time->completed && time->repeating) TimerReset(time);
    if(!time->repeating) time->completed = false;
    time->remaining -= GetFrameTime();
    if (time->remaining <= 0) time->completed = true;
}

bool TimerCompleted(const Timer *time)
{
    return time->completed;
}

Timer InitTimer(float lifetime, bool repeating)
{
    return (Timer){
        .remaining = lifetime,
        .lifetime = lifetime,
        .repeating = repeating,
        .completed = false,
        .previously_completed = false,
        .paused = false,
    };
}

bool IsTimerDone(const Timer *time)
{
    return time->lifetime > 0.;
}

f32 TimeProgress(const Timer *time)
{
    return (time->remaining / time->lifetime);
}
