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
#include "utils.h"


Game g = {0};
Assets a = {0};
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
    f32 time = GetTime();
    f32 amplitude = 10.;
    f32 speed = 1.;
    f32 offset = 80 + amplitude * sin(speed * time);
    DrawParalaxMainMenu();
    DrawTextEx(a.font, "Salvager", (Vector2) {220, offset}, 42, 4, WHITE);

    MainMenuSelectDraw();
}

void MainMenuUpdate(void)
{
    UpdateMusicStream(a.bgm);
    MainMenuSelectUpdate();
}

void GameOptionInit(void)
{
    g.camera = (Camera2D) {
        .rotation = 0,
        .zoom = 1.,
        .offset = {0, 0},
        .target = (Vector2){0, 0},
    };
    PlayMusicStream(a.bgm);
    UpdateMusicStream(a.bgm);
    SetMusicVolume(a.bgm, c.bgmVolume);
}

void GameOptionUpdate(void)
{
    g.camera = (Camera2D) {
        .rotation = 0,
        .zoom = 1.,
        .offset = {0, 0},
        .target = (Vector2){0, 0},
    };
    UpdateMusicStream(a.bgm);
    CameraShake(&g.camera, &c.shakeness, c.recover);
    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_X)) SceneChange(SCENE_MAIN_MENU);

    OptionSliderMenuUpdate();
}

void GameOptionDraw(void)
{
    BeginMode2D(g.camera);
        DrawParalaxMainMenu();
        OptionSliderMenuDraw();
    EndMode2D();
}

void UIGameDraw(void)
{
    ResourceDraw();
    Color color = GREEN;
    if (TimerCompleted(&g.player.hitShaderDelay)) {
        color = (Color){255, 0, 0, 255};
    }
    ProgressDraw((Rectangle) {0, 0, SCREEN_WIDTH, 15}, g.scrap_collected, LevelUpRequirement(), color, (Color) {255, 255, 255, 32});
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
    SetSoundVolume(a.select, c.sfxVolume);
    SetSoundVolume(a.collideAsteroid, c.sfxVolume);
    SetSoundVolume(a.engineLoop, c.sfxVolume);
    SetGenerateAsteroidInterval(InitTimer(ASTEROID_SPAWN_LIFETIME, true));
    srand(time(NULL));

    memset(g.scrap, 0, sizeof(Scrap) * SCRAP_LIMIT);
    memset(g.asteroid, 0, sizeof(Asteroid) * ASTEROID_LIMIT);

    CreateNewScrap((Vector2) {10050, 10050}, 20.);
    CreateNewAsteroid((Vector2) {10000, 10150}, (MovementParams) {{0, 0}, 5}, 4);
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
    g.elapsed = 0;
    c.shakeness = 0.,
    g.gameOver = false;
    g.multiplier_scrap = 1;

    g.paralax[0] = (Paralax) {
        .img = a.bg,
        .factor = -0.5,
    };

    g.camera = (Camera2D) {
        .rotation = 0,
        .zoom = 1.25,
        .offset = (Vector2){SCREEN_WIDTH / 2., SCREEN_HEIGHT / 2.},
        .target = (Vector2){10000, 10000},
    };
    Timer hitTimer = InitTimer(0.4, true);
    hitTimer.remaining = 0;
    hitTimer.paused = true;
    g.player = (Player) {
        .rotation = 0,
        .rotationSpeed = 250,
        .friction = PLAYER_FRICTION,
        .maxSpeed = 350,
        .position = {10000,10000},
        .acceleration = 200,
        .collisionRadius = 16.,

        .damage = 1,
        .collectionRadius = 32,
        .shotCooldown = InitTimer(DEFAULT_COOLDOWN_PLAYER_SHOT, true),
        .engineBlink = InitTimer(PLAYER_BURN_BLINK, true),
        .hitShaderDelay = hitTimer,
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

    UIGameDraw();

    if (g.paused && g.event == EVENT_NORMAL && !g.gameOver)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {32, 32, 32, 128});
        PauseSelectDraw();
    } else if (g.paused && g.event == EVENT_LEVEL_UP)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {32, 32, 32, 128});
        LevelupSelectionDraw();
    } else if (g.paused && g.gameOver) {

        GameOverMenuDraw();
    }

}

void GameSceneUpdate()
{
    UpdateMusicStream(a.bgm);

    if (g.scrap_collected - g.scrap_spent <= 0) {
        g.gameOver = true;
        g.paused = true;
    }

    if (g.event == EVENT_NORMAL) {
        if (IsKeyPressed(KEY_ESCAPE)) g.paused = !g.paused;
    }


    if (!g.paused)
    {
        g.elapsed += GetFrameTime();
        EventUpdate();
        PlayerUpdate();
        ScrapUpdate();
        ScrapPickup();
        AsteroidUpdate();
        BulletPlayerUpdate();
        BulletPlayerCollisionWithAsteroid();
        GenerateAsteroid();
        AsteroidCollision();
        CameraFollowPlayer();
        CameraShake(&g.camera, &c.shakeness, c.recover);
    } else if (g.paused && g.event == EVENT_NORMAL && !g.gameOver) {
        PauseSelectUpdate();
    } else if (g.paused && g.event == EVENT_LEVEL_UP) {
        LevelupSelectionUpdate();
    } else if (g.paused && g.gameOver) {
        GameOverMenuUpdate();
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
    a.engineLoop = LoadSound("resources/engine-loop.wav");
    a.explosiveAsteroid = LoadSound("resources/explosive-asteroid.wav");
    a.select = LoadSound("resources/select.wav");
    a.collideAsteroid = LoadSound("resources/collide-asteroid.wav");
    a.bgm = LoadMusicStream("resources/mini-jam-bgm.ogg");
    a.hitShader = LoadShader(0, "resources/hit.fs");
    c.bgmVolume = 0.8;
    c.sfxVolume = 0.6;
    c.shakeness = 0.,
    c.recover = 0.5,
    c.shaking = 1.;
    c.fullscreen = FULLSCREEN;
    GameReset();

    SceneInit(InitTimer(0.4, false));
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
    SceneSwap(SCENE_MAIN_MENU);
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
    UnloadSound(a.select);
    UnloadSound(a.collideAsteroid);
    UnloadSound(a.engineLoop);
}
