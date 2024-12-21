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
#include "bullet.h"
#include "include/logger.h"
#include "include/timer.h"

#include "./player.h"

void PlayerDraw(void)
{
    Rectangle dst = (Rectangle) {
        .x = g.player.position.x - PLAYER_SPRITE.width / 2.,
        .y = g.player.position.y - PLAYER_SPRITE.height / 2.,
        .width = PLAYER_SPRITE.width,
        .height = PLAYER_SPRITE.height,
    };
    Vector2 origin = (Vector2) {
        .x = PLAYER_SPRITE.width / 2.,
        .y = PLAYER_SPRITE.width / 2.,
    };
    DrawTexturePro(a.spriteAtlas, PLAYER_SPRITE, dst, origin, g.player.rotation, WHITE);
    /*DrawCircleV((Vector2){dst.x, dst.y}, g.player.collectionRadius, (Color){255, 0, 0, 128});*/
}

void PlayerUpdate(void)
{
    float delta = GetFrameTime();

    if (IsKeyDown(KEY_LEFT)) g.player.rotation -= g.player.rotationSpeed * delta;
    if (IsKeyDown(KEY_RIGHT)) g.player.rotation += g.player.rotationSpeed * delta;

    CLAMP_WRAPPED(g.player.rotation, 0., 360.);

    if(IsKeyDown(KEY_UP)) {
        f32 radian = (g.player.rotation - 90) * DEG2RAD;
        Vector2 forward = {cosf(radian), sinf(radian)};
        forward = Vector2Scale(forward, g.player.acceleration);
        forward = Vector2Scale(forward, delta);
        g.player.velocity = Vector2Add(forward, g.player.velocity);
    } else {
        g.player.velocity = Vector2Scale(g.player.velocity, g.player.friction);
    }

    if (Vector2Length(g.player.velocity) > g.player.maxSpeed) {
        g.player.velocity = Vector2Scale(Vector2Normalize(g.player.velocity), g.player.maxSpeed);
    }

    g.player.position = Vector2Add(Vector2Scale(g.player.velocity, delta), g.player.position);

    TimerUpdate(&g.player.shotCooldown);
    if (!TimerCompleted(&g.player.shotCooldown)) return;

    if (IsKeyDown(KEY_Z) &&  0 <= (g.scrap_collected - g.scrap_spent)) 
    {
        TimerReset(&g.player.shotCooldown);
        f32 radian = (g.player.rotation - 90) * DEG2RAD;
        Vector2 forward = {cosf(radian), sinf(radian)};
        forward = Vector2Scale(forward, BULLET_VELOCITY);
        forward = Vector2Scale(forward, delta);
        g.scrap_spent += 0.5;
        BulletPlayerCreate(g.player.position, (MovementParams) 
            {.
                velociy = forward,
                .rotationSpeed = g.player.rotation
            },
            g.player.damage
        );
        PlaySound(a.playerShot);
    }
}
