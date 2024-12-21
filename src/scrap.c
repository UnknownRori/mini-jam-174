#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "./include/salvager_internal.h"
#include "./include/types.h"
#include "./include/utils.h"
#include "include/logger.h"
#include "include/timer.h"

#include "./scrap.h"

void CreateNewScrap(Vector2 pos, f32 val)
{
    for (u32 i = 0; i < SCRAP_LIMIT; i++)
    {
        if (!g.scrap[i].active) {
            __LOG("Scrap Created");
            g.scrap[i] = (Scrap) {
                .position = pos,
                .active = true,
                .value = val,
                .timer = InitTimer(SCRAP_LIFETIME, false),
            };
            break;
        }
    }
}


void ScrapDraw(void)
{
    for (u32 i = 0; i < SCRAP_LIMIT; i++)
    {
        if (!g.scrap[i].active) continue;
        Rectangle dst = (Rectangle) {
            .x = g.scrap[i].position.x - SCRAP_SPRITE.width / 2.,
            .y = g.scrap[i].position.y - SCRAP_SPRITE.height / 2.,
            .width = SCRAP_SPRITE.width,
            .height = SCRAP_SPRITE.height,
        };
        Vector2 origin = (Vector2) {
            .x = SCRAP_SPRITE.width / 2.,
            .y = SCRAP_SPRITE.width / 2.,
        };
        DrawTexturePro(a.spriteAtlas, SCRAP_SPRITE, dst, origin, 0, WHITE);
    }
}

void ScrapUpdate(void)
{
    for (u32 i = 0; i < SCRAP_LIMIT; i++)
    {
        if (!g.scrap[i].active) continue;
        TimerUpdate(&g.scrap[i].timer);
        if (TimerCompleted(&g.scrap[i].timer)) {
            g.scrap[i].active = false;
            __LOG("Scrap despawn");
        }
    }
}

void ScrapRemove(u32 id)
{
    g.scrap[id].active = false;
}

void ScrapPickup(void)
{
    Vector2 playerSize = (Vector2) {
        .x = PLAYER_SPRITE.width,
        .y = PLAYER_SPRITE.height,
    };
    Vector2 playerPos = Vector2Subtract(g.player.position, Vector2Scale(playerSize, 0.5));
    for (u32 i = 0; i < SCRAP_LIMIT; i++)
    {
        if (g.scrap[i].active) {
            if (CheckCollisionPointCircle(g.scrap[i].position, playerPos, g.player.collectionRadius)) {
                __LOG("Scrap Collected %.0f", g.scrap[i].value);
                g.scrap_collected += g.scrap[i].value * g.multiplier_scrap;
                PlaySound(a.pickUp);
                ScrapRemove(i);
            }
        }
    }
}
