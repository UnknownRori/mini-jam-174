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

Game g = {0};
Assets a = {0};
Config c = {0};

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

    CLAMP(g.player.rotation, 0., 360.);

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

void GameLoop(void)
{
    PlayerUpdate();
    CameraFollowPlayer();

    BeginDrawing();
        ClearBackground(BLACK);

        ParalaxDraw();
        BeginMode2D(g.camera);
            ScrapDraw();
            PlayerDraw();
        EndMode2D();

        DrawFPS(0, SCREEN_HEIGHT - 24);
    EndDrawing();
}


void CreateNewScrap(Vector2 pos, u32 val)
{
    for (u32 i = 0; i < SCRAP_LIMIT; i++)
    {
        if (!g.scrap[i].active) {
            g.scrap[i] = (Scrap) {
                .position = pos,
                .active = true,
                .value = val,
            };
        }
    }
}

void GameInit(void)
{
    memset(&g, 0, sizeof(Game));

    a.spriteAtlas = LoadTexture("resources/atlas.png");
    a.bg = LoadTexture("resources/bg.png");
    a.font = LoadFont("resources/PressStart2P-Regular.ttf");

    CreateNewScrap((Vector2) {50, 50}, 20);

    g.paralax[0] = (Paralax) {
        .img = a.bg,
        .factor = -0.5,
    };

    g.camera = (Camera2D) {
        .rotation = 0,
        .zoom = 1,
        .offset = (Vector2){SCREEN_WIDTH / 2., SCREEN_HEIGHT / 2.},
        .target = (Vector2){0, 0},
    };
    g.player = (Player) {
        .rotation = 0,
        .rotationSpeed = 250,
        .friction = .95,
        .maxSpeed = 300,
        .position = VECTOR2_ZERO,
        .acceleration = 150,
    };
}
void GameUnload(void)
{
    UnloadTexture(a.spriteAtlas);
    UnloadTexture(a.bg);
}
