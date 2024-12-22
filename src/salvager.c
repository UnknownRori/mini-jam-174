#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
#include "scene.h"


Game g = {0};
Assets a = {};
Config c = {0};

bool gameShouldQuit = false;

void Quit(void)
{
    gameShouldQuit = true;
}

bool ShouldGameQuit()
{
    return gameShouldQuit;
}

void MainMenuInit(void)
{
    PlayMusicStream(a.bgm);
    SetMusicVolume(a.bgm, c.bgmVolume);
}

f32 scrollX = 0.;

void DrawParalaxMainMenu(void)
{
    f32 delta = GetFrameTime();
    f32 scrollSpeed = 200.f;
    scrollX += scrollSpeed * delta;

    if (scrollX >= SCREEN_WIDTH) scrollX -= SCREEN_WIDTH;

    DrawTexturePro(
        a.bg, 
        (Rectangle) {
            .x = 0, 
            .y = 0, 
            .width = a.bg.width, 
            .height = a.bg.height
        }, 
        (Rectangle) {
            .x = -scrollX,
            .y = 0,
            .width = SCREEN_WIDTH,
            .height = SCREEN_HEIGHT,
        }, 
        VECTOR2_ZERO, 
        0., 
        WHITE
    );
    DrawTexturePro(
        a.bg, 
        (Rectangle) {
            .x = 0, 
            .y = 0, 
            .width = a.bg.width, 
            .height = a.bg.height
        }, 
        (Rectangle) {
            .x = SCREEN_WIDTH - scrollX,
            .y = 0,
            .width = SCREEN_WIDTH,
            .height = SCREEN_HEIGHT,
        }, 
        VECTOR2_ZERO, 
        0., 
        WHITE
    );
}

void MainMenuDraw(void)
{
    DrawParalaxMainMenu();

    MainMenuSelectDraw();
}

void MainMenuUpdate(void)
{
    UpdateMusicStream(a.bgm);
    MainMenuSelectUpdate();
}

void GameOptionInit(void)
{
    PlayMusicStream(a.bgm);
    UpdateMusicStream(a.bgm);
    SetMusicVolume(a.bgm, c.bgmVolume);
}

void GameOptionUpdate(void)
{
    UpdateMusicStream(a.bgm);
    if (IsKeyPressed(KEY_ESCAPE)) SceneChange(SCENE_MAIN_MENU);

    OptionSliderMenuUpdate();
}

void GameOptionDraw(void)
{
    DrawParalaxMainMenu();
    OptionSliderMenuDraw();
}

void UIGameDraw(void)
{
    ResourceDraw();
    ProgressDraw((Rectangle) {0, 0, SCREEN_WIDTH, 15}, g.scrap_collected, LevelUpRequirement(), GREEN, (Color) {255, 255, 255, 32});
}


void CameraFollowPlayer(void)
{
    g.camera.target = g.player.position;
}

void GameReset()
{
    PlayMusicStream(a.bgm);
    SetMusicVolume(a.bgm, c.bgmVolume);
    SetSoundVolume(a.explosiveAsteroid, c.sfxVolume);
    SetSoundVolume(a.hit, c.sfxVolume);
    SetSoundVolume(a.pickUp, c.sfxVolume);
    SetSoundVolume(a.playerShot, c.sfxVolume);
    SetGenerateAsteroidInterval(InitTimer(0.8, true));
    srand(time(NULL));

    memset(g.scrap, 0, sizeof(Scrap) * SCRAP_LIMIT);
    memset(g.asteroid, 0, sizeof(Asteroid) * ASTEROID_LIMIT);

    CreateNewScrap((Vector2) {50, 50}, 20.);
    CreateNewAsteroid((Vector2) {-80, 80}, (MovementParams) {{0, 0}, 5}, 4);
    /*CreateNewAsteroid((Vector2) {-50, 20}, (MovementParams) {{-10, 5}, 5}, 4);*/
    /*CreateNewAsteroid((Vector2) {-20, 40}, (MovementParams) {{10, -5}, 5}, 4);*/
    /*CreateNewAsteroid((Vector2) {-24, 20}, (MovementParams) {{-10, -5}, 5}, 4);*/
    /*CreateNewAsteroid((Vector2) {-38, 40}, (MovementParams) {{20, -5}, 5}, 4);*/
    /*CreateNewAsteroid((Vector2) {-18, 40}, (MovementParams) {{20, -5}, 5}, 4);*/
    g.paused = false;
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
        .collisionRadius = 16.,

        .damage = 1,
        .collectionRadius = 32,
        .shotCooldown = InitTimer(DEFAULT_COOLDOWN_PLAYER_SHOT, true),
        .engineBlink = InitTimer(PLAYER_BURN_BLINK, true),
    };
}

void GameSceneDraw()
{
    ParalaxDraw();
    BeginMode2D(g.camera);
        ScrapDraw();
        AsteroidDraw();
        BulletPlayerDraw();
        PlayerDraw();
    EndMode2D();

    if (g.paused && g.event == EVENT_NORMAL)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {32, 32, 32, 128});
        PauseSelectDraw();
    } else if (g.paused && g.event == EVENT_LEVEL_UP)
    {
        LevelupSelectionDraw();
    }

    UIGameDraw();
}

void GameSceneUpdate()
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
        AsteroidCollision();
        CameraFollowPlayer();
    } else if (g.paused && g.event == EVENT_NORMAL) {
        PauseSelectUpdate();
    } else if (g.paused && g.event == EVENT_LEVEL_UP) {
        LevelupSelectionUpdate();
    }

}

void GameSceneInit()
{
    GameReset();
}


void GameLoop(void)
{
    SceneUpdate();
    BeginDrawing();
        ClearBackground(BLACK);

        SceneDraw();

        DrawFPS(0, SCREEN_HEIGHT - 24);
        DrawTextEx(a.font, GAME_VERSION, (Vector2) {SCREEN_WIDTH - 58, SCREEN_HEIGHT - 21}, 14, 4, (Color){255, 255, 255, 128});
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
    c.bgmVolume = 0.8;
    c.sfxVolume = 0.6;
    GameReset();

    SceneAdd(SCENE_MAIN_MENU, (Scene) {
        .init = MainMenuInit,
        .draw = MainMenuDraw,
        .update = MainMenuUpdate,
    });
    SceneAdd(SCENE_OPTION, (Scene) {
        .init = GameOptionInit,
        .draw = GameOptionDraw,
        .update = GameOptionUpdate,
    });
    SceneAdd(SCENE_GAME, (Scene) {
        .init = GameSceneInit,
        .draw = GameSceneDraw,
        .update = GameSceneUpdate,
    });
    SceneChange(SCENE_MAIN_MENU);
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
