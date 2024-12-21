#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "./include/salvager.h"
#include "./include/salvager_internal.h"
#include "./asteroid.h"
#include "./player.h"
#include "./scrap.h"
#include "./ui.h"
#include "./include/types.h"
#include "./include/utils.h"
#include "bullet.h"
#include "include/logger.h"
#include "include/timer.h"


Game g = {0};
Assets a = {};
Config c = {0};

void UIGameDraw(void)
{
    ResourceDraw();
    ProgressDraw((Rectangle) {0, 0, SCREEN_WIDTH, 15}, g.scrap_collected, 100, GREEN, (Color) {255, 255, 255, 32});
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


void GameLoop(void)
{
    PlayerUpdate();
    ScrapUpdate();
    ScrapPickup();
    AsteroidUpdate();
    BulletPlayerUpdate();
    BulletPlayerCollisionWithAsteroid();
    CameraFollowPlayer();

    BeginDrawing();
        ClearBackground(BLACK);

        ParalaxDraw();
        BeginMode2D(g.camera);
            ScrapDraw();
            AsteroidDraw();
            BulletPlayerDraw();
            PlayerDraw();
        EndMode2D();

        UIGameDraw();
        DrawFPS(0, SCREEN_HEIGHT - 24);
    EndDrawing();
}

void GameInit(void)
{
    memset(&g, 0, sizeof(Game));

    a.spriteAtlas = LoadTexture("resources/atlas.png");
    a.bg = LoadTexture("resources/bg.png");
    a.font = LoadFont("resources/PressStart2P-Regular.ttf");

    CreateNewScrap((Vector2) {50, 50}, 20.);
    CreateNewAsteroid((Vector2) {-80, 80}, (MovementParams) {{10, 5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-50, 20}, (MovementParams) {{-10, 5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-20, 40}, (MovementParams) {{10, -5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-24, 20}, (MovementParams) {{-10, -5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-38, 40}, (MovementParams) {{20, -5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-18, 40}, (MovementParams) {{20, -5}, 5}, 4);
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

        .damage = 1,
        .collecionRadius = 32,
        .shotCooldown = InitTimer(DEFAULT_COOLDOWN_PLAYER_SHOT, true),
    };
}
void GameUnload(void)
{
    UnloadTexture(a.spriteAtlas);
    UnloadTexture(a.bg);
}
