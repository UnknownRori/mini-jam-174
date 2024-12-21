#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "./include/salvager.h"
#include "./include/salvager_internal.h"
#include "./include/types.h"
#include "./include/utils.h"
#include "./scrap.h"
#include "include/logger.h"
#include "include/timer.h"

#include "./asteroid.h"

static Timer interval = {0};

Rectangle static GetSpriteAsteroid(int index)
{
    switch(index) {
        case 0:
            return ASTEROID_SPRITE1;
        case 1:
            return ASTEROID_SPRITE2;
        default:
            return ASTEROID_SPRITE1;
    }
}

void CreateNewAsteroid(Vector2 pos, MovementParams move, f32 hp)
{
    int rand = GetRandomValue(0, 1);
    for (u32 i = 0; i < ASTEROID_LIMIT; i++)
    {
        if (g.asteroid[i].active) continue;
        __LOG("Asteroid Created");

        g.asteroid[i] = (Asteroid) {
            .type = rand,
            .active = true,
            .sprite = GetSpriteAsteroid(rand),
            .lifetime = InitTimer(ASTEROID_LIFETIME, false),
            .position = pos,
            .hitboxRadius = 16,
            .scrap = 12,
            .rotationSpeed = move.rotationSpeed,
            .velocity = move.velociy,
            .rotation = 0,
            .hp = hp,
        };

        break;
    }
}


void AsteroidDraw(void)
{
    for (u32 i = 0; i < ASTEROID_LIMIT ; i++)
    {
        if (!g.asteroid[i].active) continue;
        
        Rectangle dst = (Rectangle) {
            .x = g.asteroid[i].position.x - g.asteroid[i].sprite.width / 2.,
            .y = g.asteroid[i].position.y - g.asteroid[i].sprite.height / 2.,
            .width = g.asteroid[i].sprite.width,
            .height = g.asteroid[i].sprite.height,
        };
        Vector2 origin = (Vector2) {
            .x = g.asteroid[i].sprite.width / 2.,
            .y = g.asteroid[i].sprite.width / 2.,
        };
        DrawTexturePro(a.spriteAtlas, g.asteroid[i].sprite, dst, origin, g.asteroid[i].rotation, WHITE);
        /*DrawCircleV((Vector2) {dst.x, dst.y}, g.asteroid[i].hitboxRadius, (Color) {255, 0, 0, 128});*/
    }
}

void AsteroidUpdate(void)
{
    f32 delta = GetFrameTime();
    for (u32 i = 0; i < ASTEROID_LIMIT; i++)
    {
        if (!g.asteroid[i].active) continue;

        if (g.asteroid[i].hp < 0)
        {
            g.asteroid[i].active = false;
            PlaySound(a.explosiveAsteroid);
            CreateNewScrap(g.asteroid[i].position, g.asteroid[i].scrap);
            continue;
        }
        g.asteroid[i].rotation += g.asteroid[i].rotationSpeed * delta;
        g.asteroid[i].position = Vector2Add(Vector2Scale(g.asteroid[i].velocity, delta), g.asteroid[i].position);
        CLAMP_WRAPPED(g.asteroid[i].rotation, 0., 360.);

        TimerUpdate(&g.asteroid[i].lifetime);
        if (TimerCompleted(&g.asteroid[i].lifetime)) {
            g.asteroid[i].active = false;
            __LOG("Asteroid (%d) despawn", i);
        }
    }
}

void SetGenerateAsteroidInterval(Timer time){}
void GenerateAsteroid(){}
