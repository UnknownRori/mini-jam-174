#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "./include/salvager.h"
#include "./include/salvager_internal.h"
#include "./include/types.h"
#include "./include/utils.h"
#include "./scrap.h"
#include "include/logger.h"
#include "include/timer.h"
#include "physic.h"
#include "utils.h"

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

        Timer timer = InitTimer(2, true);
        timer.remaining = 0;
        Timer hitTimer = InitTimer(0.4, true);
        hitTimer.paused = true;
        hitTimer.remaining = 0;
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
            .playerHitTimer = timer,
            .hitShaderDelay = hitTimer,
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
        if (TimerCompleted(&g.asteroid[i].hitShaderDelay)) {
            BeginShaderMode(a.hitShader);
            DrawTexturePro(a.spriteAtlas, g.asteroid[i].sprite, dst, origin, g.asteroid[i].rotation, WHITE);
            EndShaderMode();
        } else {
            DrawTexturePro(a.spriteAtlas, g.asteroid[i].sprite, dst, origin, g.asteroid[i].rotation, WHITE);
        }
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
            c.shakeness = c.shaking;
            CreateNewScrap(g.asteroid[i].position, g.asteroid[i].scrap);
            continue;
        }
        g.asteroid[i].rotation += g.asteroid[i].rotationSpeed * delta;
        g.asteroid[i].position = Vector2Add(Vector2Scale(g.asteroid[i].velocity, delta), g.asteroid[i].position);
        CLAMP_WRAPPED(g.asteroid[i].rotation, 0., 360.);

        Rectangle asteroidRect = (Rectangle) {
            .x = g.asteroid[i].position.x,
            .y = g.asteroid[i].position.y,
            .width = g.asteroid[i].sprite.width,
            .height = g.asteroid[i].sprite.height,
        };

        if (TimerCompleted(&g.asteroid[i].hitShaderDelay)) {
            TimerReset(&g.asteroid[i].hitShaderDelay);
            g.asteroid[i].hitShaderDelay.remaining = 0;
            g.asteroid[i].hitShaderDelay.paused = true;
        }

        TimerUpdate(&g.asteroid[i].hitShaderDelay);
        if (CheckIfInGameSpace(asteroidRect, g.camera, SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            TimerUpdate(&g.asteroid[i].lifetime);
            if (TimerCompleted(&g.asteroid[i].lifetime)) {
                g.asteroid[i].active = false;
                __LOG("Asteroid (%d) despawn", i);
            }
        }
    }
}

void SetGenerateAsteroidInterval(Timer time){
    interval = time;
}

void GenerateAsteroid(){
    TimerUpdate(&interval);
    if (!TimerCompleted(&interval)) return;
    int region = GetRandomValue(0, 3);
    Rectangle rect = GetCameraBounds(g.camera, SCREEN_WIDTH, SCREEN_HEIGHT);
    Vector2 randomPoint;
    float buffer = ASTEROID_SPAWN_PADDING;
    switch (region) {
        case 0: // Left of the rectangle
            randomPoint.x = GetRandomValue(rect.x - buffer, rect.x - 1);
            randomPoint.y = GetRandomValue(rect.y - buffer, rect.y + rect.height + buffer);
            break;
        case 1: // Right of the rectangle
            randomPoint.x = GetRandomValue(rect.x + rect.width + 1, rect.x + rect.width + buffer);
            randomPoint.y = GetRandomValue(rect.y - buffer, rect.y + rect.height + buffer);
            break;
        case 2: // Above the rectangle
            randomPoint.x = GetRandomValue(rect.x - buffer, rect.x + rect.width + buffer);
            randomPoint.y = GetRandomValue(rect.y - buffer, rect.y - 1);
            break;
        case 3: // Below the rectangle
            randomPoint.x = GetRandomValue(rect.x - buffer, rect.x + rect.width + buffer);
            randomPoint.y = GetRandomValue(rect.y + rect.height + 1, rect.y + rect.height + buffer);
            break;
    }
    Vector2 dir = Vector2Subtract(randomPoint, g.player.position);
    if (Vector2Length(dir) > 0) {
        dir = Vector2Normalize(dir);
    }
    Vector2 vel = Vector2Scale(Vector2Scale(dir, -1), GetRandomValue(0, 50));

    CreateNewAsteroid(randomPoint, (MovementParams) {.velociy = vel, .rotationSpeed = GetRandomValue(0, 360)}, 4);
}


void  AsteroidCollision(void)
{
    for (u32 i = 0; i < ASTEROID_LIMIT; i++)
    {
        if (!g.asteroid[i].active) continue;
        Vector2 centerAsteroid = Vector2Subtract(g.asteroid[i].position, (Vector2) {
            .x = ASTEROID_SPRITE1.width / 2.,
            .y = ASTEROID_SPRITE1.height / 2.,
        });

        for (u32 x = i + 1; x < ASTEROID_LIMIT; x++) {
            if (!g.asteroid[x].active) continue;
            Vector2 centerAsteroid2 = Vector2Subtract(g.asteroid[x].position, (Vector2) {
                .x = ASTEROID_SPRITE1.width / 2.,
                .y = ASTEROID_SPRITE1.height / 2.,
            });

            if (CheckCollisionCircles(centerAsteroid, g.asteroid[i].hitboxRadius, centerAsteroid2, g.asteroid[x].hitboxRadius)) {
                PhysicCircleObject asteroid = (PhysicCircleObject) {
                    .position = g.asteroid[i].position,
                    .velocity = g.asteroid[i].velocity,
                };
                PhysicCircleObject asteroid2 = (PhysicCircleObject) {
                    .position = g.asteroid[x].position,
                    .velocity = g.asteroid[x].velocity,
                };
                PushCircleObject(&asteroid, &asteroid2);
                g.asteroid[i].velocity = asteroid.velocity;
                g.asteroid[i].position = asteroid.position;
                g.asteroid[x].velocity = asteroid2.velocity;
                g.asteroid[x].position = asteroid2.position;
            }
        }

        Vector2 centerPlayer = Vector2Subtract(g.player.position, (Vector2) {
            .x = PLAYER_SPRITE.width / 2.,
            .y = PLAYER_SPRITE.height / 2.,
        });


        if (CheckCollisionCircles(centerAsteroid, g.asteroid[i].hitboxRadius, centerPlayer, g.player.collisionRadius)) {
            TimerUpdate(&g.asteroid[i].playerHitTimer);
            PhysicCircleObject asteroid = (PhysicCircleObject) {
                .position = g.asteroid[i].position,
                .velocity = g.asteroid[i].velocity,
            };
            PhysicCircleObject player = (PhysicCircleObject) {
                .position = g.player.position,
                .velocity = g.player.velocity,
            };
            PushCircleObject(&asteroid, &player);
            g.asteroid[i].velocity = asteroid.velocity;
            g.asteroid[i].position = asteroid.position;
            g.player.velocity = player.velocity;
            g.player.position = player.position;
            if (TimerCompleted(&g.asteroid[i].playerHitTimer)) {
                g.player.hitShaderDelay.paused = false;
                g.scrap_collected -= 0.5;
                PlaySound(a.collideAsteroid);
            }
        }
    }
}
