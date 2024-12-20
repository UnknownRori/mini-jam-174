#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <string.h>

#include "./include/salvager.h"
#include "./include/salvager_internal.h"
#include "./include/types.h"
#include "./include/utils.h"

Game g = {0};
Assets a = {0};
Config c = {0};

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
        float radian = (g.player.rotation - 90) * DEG2RAD;
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

    BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(g.camera);
            PlayerDraw();
        EndMode2D();

        DrawFPS(0, SCREEN_HEIGHT - 24);
    EndDrawing();
}
void GameInit(void)
{
    memset(&g, 0, sizeof(Game));

    a.spriteAtlas = LoadTexture("resources/atlas.png");
    a.bg = LoadTexture("resources/bg.png");
    a.font = LoadFont("resources/PressStart2P-Regular.ttf");

    g.camera = (Camera2D) {
        .rotation = 0,
        .zoom = 2,
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
