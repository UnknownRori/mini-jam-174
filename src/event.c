#include "./include/salvager_internal.h"
#include "include/logger.h"
#include "event.h"

void EventUpdate(void)
{
    if (g.scrap_collected >= LevelUpRequirement() && g.event != EVENT_LEVEL_UP) {
        __LOG("Level up triggered");
        g.event = EVENT_LEVEL_UP;
        g.paused = true;
    }
}

f32 LevelUpRequirement()
{
    return LEVEL_REQUIREMENT * g.level;
}
