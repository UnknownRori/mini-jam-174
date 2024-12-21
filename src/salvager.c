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
#include "include/logger.h"
#include "include/timer.h"

Game g = {0};
Assets a = {0};
Config c = {0};

void ResourceDraw(void)
{
    Rectangle dst = (Rectangle) {
        .x = 18,
        .y = 24,
        .width = SCRAP_SPRITE.width + 12,
        .height = SCRAP_SPRITE.height + 12,
    };
    Vector2 origin = (Vector2) {
        .x = SCRAP_SPRITE.width / 2.,
        .y = SCRAP_SPRITE.width / 2.,
    };
    DrawTexturePro(a.spriteAtlas, SCRAP_SPRITE, dst, origin, 0, WHITE);

    char buffer[512] ={0};
    snprintf(buffer, sizeof(buffer), "%.0lf", g.scrap_collected - g.scrap_spent);

    DrawTextEx(a.font, buffer, (Vector2) {SCRAP_SPRITE.width + 24, SCRAP_SPRITE.height / 2. + 12}, 24, 4, WHITE);
}

void ProgressDraw(Rectangle size, f32 current, f32 max, Color active, Color deactive)
{
    f32 currentPercentage = current / max;
    CLAMP(currentPercentage, 0, 1);
    float activeWidth = (size.width * currentPercentage);


    DrawRectangleRec(size, deactive);
    DrawRectangleRec((Rectangle) {
        .x = size.x, 
        .y = size.y, 
        .width = activeWidth, 
        .height = size.height
    }, active);
}

void UIGameDraw(void)
{
    ResourceDraw();
    ProgressDraw((Rectangle) {0, 0, SCREEN_WIDTH, 15}, g.scrap_collected, 100, GREEN, (Color) {255, 255, 255, 32});
}

void AsteroidDraw(void)
{
    for (u32 i = 0; i < ASTEROID_LIMIT ; i++)
    {
        if (!g.asteroid[i].active) continue;
        Rectangle dst = (Rectangle) {
            .x = g.asteroid[i].position.x - ASTEROID_SPRITE1.width / 2.,
            .y = g.asteroid[i].position.y - ASTEROID_SPRITE1.height / 2.,
            .width = ASTEROID_SPRITE1.width,
            .height = ASTEROID_SPRITE1.height,
        };
        Vector2 origin = (Vector2) {
            .x = ASTEROID_SPRITE1.width / 2.,
            .y = ASTEROID_SPRITE1.width / 2.,
        };
        DrawTexturePro(a.spriteAtlas, ASTEROID_SPRITE1, dst, origin, 0, WHITE);
    }
}

void AsteroidUpdate(void)
{
    for (u32 i = 0; i < ASTEROID_LIMIT; i++)
    {
        if (!g.asteroid[i].active) continue;

        TimerUpdate(&g.scrap[i].timer);
        if (TimerCompleted(&g.scrap[i].timer)) {
            g.asteroid[i].active = false;
            __LOG("Asteroid (%d) despawn", i);
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

void ParalaxDraw(void)
{
    const Vector2 cameraPos = g.camera.target;
    /*BeginBlendMode(BLEND_ALPHA);*/
    for (u32 i = 0; i < PARALAX_MAX; i++)
    {
        const Paralax *paralax = &g.paralax[i];
        f32 wrappedX = fmodf(cameraPos.x * paralax->factor, paralax->img.width);
        f32 wrappedY = fmodf(cameraPos.y * paralax->factor, paralax->img.height);
        if (wrappedX < 0) wrappedX += paralax->img.width;
        if (wrappedY < 0) wrappedX += paralax->img.height;
        for (i32 x = PARALAX_MIN; x <= SCREEN_WIDTH / paralax->img.width + 2; x++)
        {
            for (i32 y = PARALAX_MIN; y < SCREEN_HEIGHT / paralax->img.height + 2; y++)
            {
                DrawTexture(
                    paralax->img, 
                    wrappedX + x * paralax->img.width, 
                    wrappedY + y * paralax->img.height, 
                    WHITE
                );
            }
        }
    }
    /*EndBlendMode();*/
}

void CameraFollowPlayer(void)
{
    g.camera.target = g.player.position;
}

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
    DrawCircleV(playerPos, g.player.collecionRadius, (Color){255, 0, 0, 128});
    for (u32 i = 0; i < SCRAP_LIMIT; i++)
    {
        if (g.scrap[i].active) {
            if (CheckCollisionPointCircle(g.scrap[i].position, playerPos, g.player.collecionRadius)) {
                __LOG("Scrap Collected %.0f", g.scrap[i].value);
                g.scrap_collected += g.scrap[i].value;
                ScrapRemove(i);
            }
        }
    }
}

void GameLoop(void)
{
    PlayerUpdate();
    ScrapUpdate();
    ScrapPickup();
    AsteroidUpdate();
    CameraFollowPlayer();

    BeginDrawing();
        ClearBackground(BLACK);

        ParalaxDraw();
        BeginMode2D(g.camera);
            ScrapDraw();
            AsteroidDraw();
            PlayerDraw();
        EndMode2D();

        UIGameDraw();
        DrawFPS(0, SCREEN_HEIGHT - 24);
    EndDrawing();
}

void CreateNewAsteroid(Vector2 pos, MovementParams move)
{
    for (u32 i = 0; i < ASTEROID_LIMIT; i++)
    {
        if (g.asteroid[i].active) continue;
        __LOG("Asteroid Created");

        g.asteroid[i] = (Asteroid) {
            .active = true,
            .lifetime = InitTimer(ASTEROID_LIFETIME, false),
            .position = pos,
            .rotationSpeed = move.rotationSpeed,
            .velocity = move.velociy,
            .rotation = 0,
        };

        break;
    }
}

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

void GameInit(void)
{
    memset(&g, 0, sizeof(Game));

    a.spriteAtlas = LoadTexture("resources/atlas.png");
    a.bg = LoadTexture("resources/bg.png");
    a.font = LoadFont("resources/PressStart2P-Regular.ttf");

    CreateNewScrap((Vector2) {50, 50}, 20.);
    CreateNewAsteroid((Vector2) {-80, 80}, (MovementParams) {{10, 5}, 5});
    g.scrap_collected = 20.;
    g.scrap_spent = 0.;

    g.paralax[0] = (Paralax) {
        .img = a.bg,
        .factor = -0.5,
    };

    g.camera = (Camera2D) {
        .rotation = 0,
        .zoom = 1.25,
        .offset = (Vector2){SCREEN_WIDTH / 2., SCREEN_HEIGHT / 2.},
        .target = (Vector2){0, 0},
    };
    g.player = (Player) {
        .rotation = 0,
        .rotationSpeed = 250,
        .friction = .95,
        .maxSpeed = 350,
        .position = VECTOR2_ZERO,
        .acceleration = 200,
        .collecionRadius = 32,
    };
}
void GameUnload(void)
{
    UnloadTexture(a.spriteAtlas);
    UnloadTexture(a.bg);
}
