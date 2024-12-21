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
#include "./paralax.h"
#include "./include/types.h"
#include "./include/utils.h"
#include "bullet.h"
#include "event.h"
#include "include/logger.h"
#include "include/timer.h"


Game g = {0};
Assets a = {};
Config c = {0};

void UIGameDraw(void)
{
    ResourceDraw();
    ProgressDraw((Rectangle) {0, 0, SCREEN_WIDTH, 15}, g.scrap_collected, LevelUpRequirement(), GREEN, (Color) {255, 255, 255, 32});
}


void CameraFollowPlayer(void)
{
    g.camera.target = g.player.position;
}


void GameLoop(void)
{
    UpdateMusicStream(a.bgm);
    if (g.event == EVENT_NORMAL) {
        if (IsKeyPressed(KEY_ESCAPE)) g.paused = !g.paused;
    }

    EventUpdate();

    if (!g.paused)
    {
        PlayerUpdate();
        ScrapUpdate();
        ScrapPickup();
        AsteroidUpdate();
        BulletPlayerUpdate();
        BulletPlayerCollisionWithAsteroid();
        GenerateAsteroid();
        CameraFollowPlayer();
    }

    if (g.event == EVENT_LEVEL_UP)
    {
        LevelupSelectionUpdate();
    }

    BeginDrawing();
        ClearBackground(BLACK);

        ParalaxDraw();
        BeginMode2D(g.camera);
            ScrapDraw();
            AsteroidDraw();
            BulletPlayerDraw();
            PlayerDraw();
        EndMode2D();

        if (g.paused)
        {
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {32, 32, 32, 128});
        }

        if (g.event == EVENT_LEVEL_UP)
        {
            LevelupSelectionDraw();
        }

        UIGameDraw();
        DrawFPS(0, SCREEN_HEIGHT - 24);
    EndDrawing();
}

void GameInit(void)
{
    memset(&g, 0, sizeof(Game));

    SetExitKey(KEY_F1);

    a.spriteAtlas = LoadTexture("resources/atlas.png");
    a.bg = LoadTexture("resources/bg.png");
    a.font = LoadFont("resources/PressStart2P-Regular.ttf");
    a.playerShot = LoadSound("resources/playershot.wav");
    a.pickUp = LoadSound("resources/pickup.wav");
    a.hit = LoadSound("resources/hit.wav");
    a.explosiveAsteroid = LoadSound("resources/explosive-asteroid.wav");
    a.bgm = LoadMusicStream("resources/mini-jam-bgm.ogg");
    PlayMusicStream(a.bgm);
    SetMusicVolume(a.bgm, 1.);

    CreateNewScrap((Vector2) {50, 50}, 20.);
    CreateNewAsteroid((Vector2) {-80, 80}, (MovementParams) {{10, 5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-50, 20}, (MovementParams) {{-10, 5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-20, 40}, (MovementParams) {{10, -5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-24, 20}, (MovementParams) {{-10, -5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-38, 40}, (MovementParams) {{20, -5}, 5}, 4);
    CreateNewAsteroid((Vector2) {-18, 40}, (MovementParams) {{20, -5}, 5}, 4);
    g.scrap_collected = 20.;
    g.scrap_spent = 0.;
    g.event = EVENT_NORMAL;
    g.scene = SCENE_GAME;
    g.level = 1;
    g.multiplier_scrap = 1;

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
        .collectionRadius = 32,
        .shotCooldown = InitTimer(DEFAULT_COOLDOWN_PLAYER_SHOT, true),
    };
}
void GameUnload(void)
{
    UnloadTexture(a.spriteAtlas);
    UnloadTexture(a.bg);
    UnloadMusicStream(a.bgm);
    UnloadSound(a.playerShot);
    UnloadSound(a.hit);
    UnloadSound(a.explosiveAsteroid);
    UnloadSound(a.pickUp);
}
