#include "./bullet.h"
#include "include/logger.h"
#include "include/salvager_internal.h"
#include "include/timer.h"
#include "include/types.h"
#include <raylib.h>
#include <raymath.h>


void BulletPlayerCreate(Vector2 pos, MovementParams move, f32 damage)
{
    for (u32 i = 0; i < BULLET_LIMIT; i++)
    {
        if (g.playerBullet[i].active) continue;

        g.playerBullet[i] = (PlayerBullet) {
            .active = true,
            .damage = damage,
            .lifetime = InitTimer(BULLET_LIFETIME, false),
            .position = pos,
            .velocity = move.velociy,
            .rotation = move.rotationSpeed,
        };
        break;
    }
}

void BulletPlayerDraw(void)
{

    for (u32 i = 0; i < BULLET_LIMIT; i++)
    {
        if (!g.playerBullet[i].active) continue;

        Rectangle dst = (Rectangle) {
            .x = g.playerBullet[i].position.x - PLAYER_BULLET_SPRITE.width / 2.,
            .y = g.playerBullet[i].position.y - PLAYER_BULLET_SPRITE.height / 2.,
            .width = PLAYER_BULLET_SPRITE.width,
            .height = PLAYER_BULLET_SPRITE.height,
        };
        Vector2 origin = (Vector2) {
            .x = PLAYER_BULLET_SPRITE.width / 2.,
            .y = PLAYER_BULLET_SPRITE.width / 2.,
        };
        DrawTexturePro(a.spriteAtlas, PLAYER_BULLET_SPRITE, dst, origin, g.playerBullet[i].rotation, WHITE);
    }
}
void BulletPlayerUpdate(void)
{
    for (u32 i = 0; i < BULLET_LIMIT; i++)
    {
        if (!g.playerBullet[i].active) continue;

        TimerUpdate(&g.playerBullet[i].lifetime);
        g.playerBullet[i].position = Vector2Add(g.playerBullet[i].position, g.playerBullet[i].velocity);
        if (TimerCompleted(&g.playerBullet[i].lifetime)) {
            g.playerBullet[i].active = false;
            __LOG("Player Bullet (%d) despawn", i);
        }
    }
}
void BulletPlayerCollisionWithAsteroid(void)
{
    // INFO : Not so efficient
    for (u32 bi = 0; bi < BULLET_LIMIT; bi++)
    {
        if (!g.playerBullet[bi].active) continue;
        for (u32 ai = 0; ai < ASTEROID_LIMIT; ai++)
        {
            if (!g.asteroid[ai].active) continue;
            Vector2 centerAsteroid = Vector2Subtract(g.asteroid[ai].position, (Vector2) {
                .x = ASTEROID_SPRITE1.width / 2.,
                .y = ASTEROID_SPRITE1.height / 2.,
            });
            Vector2 centerBullet = Vector2Subtract(g.playerBullet[bi].position, (Vector2) {
                .x = PLAYER_BULLET_SPRITE.width / 2.,
                .y = PLAYER_BULLET_SPRITE.height / 2.,
            });
            if (CheckCollisionPointCircle(centerBullet, centerAsteroid, g.asteroid[ai].hitboxRadius))
            {
                __LOG("Hit");
                PlaySound(a.hit);
                g.asteroid[ai].hitShaderDelay.paused = false;
                g.playerBullet[bi].active = false;
                g.asteroid[ai].hp -= g.playerBullet[bi].damage;
            }
        }
    }
}
