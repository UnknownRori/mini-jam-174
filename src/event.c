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

void LevelUpPlayer(LevelType l)
{
    g.event = EVENT_NORMAL;
    g.paused = false;
    g.level += 1;
    switch (l) {
        case LEVEL_UP_COLLECTION_RANGE:
            g.player.collectionRadius += 20;
        break;
        case LEVEL_UP_FAST_FIRE:
            g.player.shotCooldown.lifetime *= 0.8;
        break;
        case LEVEL_UP_MULTIPLY_DROP_RATE:
            g.multiplier_scrap *= 2.2;
        break;
    }
}
