#pragma once

#ifndef SALVAGER_INTERNAL_H
#define SALVAGER_INTERNAL_H

#include <raylib.h>
#include "./salvager.h"
#include "./timer.h"
#include "./types.h"
#include "./utils.h"

#define GAME_VERSION "0.0"

#define ASTEROID_LIMIT 256
#define ASTEROID_LIFETIME 80
#define ASTEROID_SPAWN_PADDING 200

#define SCRAP_LIMIT 512
#define SCRAP_LIFETIME 30

#define BULLET_LIMIT 512
#define BULLET_LIFETIME 5
#define BULLET_VELOCITY 300

#define PARALAX_LIMIT 1
#define PARALAX_MIN -2
#define PARALAX_MAX 1

/*#define DEFAULT_COOLDOWN_PLAYER_SHOT 0.8*/
#define DEFAULT_COOLDOWN_PLAYER_SHOT 0.1
#define LEVEL_REQUIREMENT 100
#define LEVEL_MULTIPLIER 2

#define PLAYER_BURN_BLINK 0.05

typedef struct {
    Vector2 position;
    Vector2 velocity;

    f32 acceleration;
    f32 friction;

    f32 maxSpeed;

    f32 rotation;
    f32 rotationSpeed;

    f32 collisionRadius;

    f32 collectionRadius;
    f32 damage;

    Timer hitShaderDelay;
    Timer engineBlink;
    Timer shotCooldown;
} Player;

typedef struct
{
    Vector2 velociy;
    f32 rotationSpeed;
} MovementParams;

typedef enum {
    ASTEROID_SMALL_1,
    ASTEROID_SMALL_2,
} AsteroidType;


typedef struct {
    AsteroidType type;
    Rectangle sprite;

    Vector2 position;
    Vector2 velocity;

    f32 rotation;
    f32 rotationSpeed;

    f32 hp;
    f32 scrap;
    f32 hitboxRadius;
    Timer lifetime;
    Timer hitShaderDelay;
    Timer playerHitTimer;
    bool active;
} Asteroid;

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    f32 rotation;
    f32 damage;

    Timer lifetime;
    bool active;
} PlayerBullet;

typedef struct {
    Vector2 position;
    f32 value;

    bool active;
    Timer timer;
} Scrap;

typedef struct
{
    Texture2D img;
    f32 factor;
} Paralax;

typedef enum
{
    SCENE_MAIN_MENU,
    SCENE_GAME, 
    SCENE_OPTION, 
} GameScene;

typedef enum
{
    EVENT_NORMAL,
    EVENT_LEVEL_UP, 
} GameEvent;

typedef struct {
    Player player;
    Asteroid asteroid[ASTEROID_LIMIT];
    Scrap scrap[SCRAP_LIMIT];
    PlayerBullet playerBullet[BULLET_LIMIT];

    u64 highscore;
    u64 score;
    f32 scrap_collected;
    f32 scrap_spent;
    u32 level;
    f32 multiplier_scrap;
    f32 elapsed;
    Paralax paralax[PARALAX_LIMIT];

    Camera2D camera;

    bool paused;
    GameScene scene;
    GameEvent event;
} Game;

typedef struct {
    f32 sfxVolume;
    f32 bgmVolume;
} Config;

#define PLAYER_SPRITE ((Rectangle) {0., 0., 32., 32.})
#define ENGINE_EXHAUST_SPRITE ((Rectangle) {160., 0., 32., 32.})
#define SCRAP_SPRITE ((Rectangle) {192., 0., 32., 32.})
#define TIME_SPRITE ((Rectangle) {224., 0., 32., 32.})
#define ASTEROID_SPRITE1 ((Rectangle) {0., 64., 32., 32.})
#define ASTEROID_SPRITE2 ((Rectangle) {32., 64., 32., 32.})
#define PLAYER_BULLET_SPRITE ((Rectangle) {128., 0., 32., 32.})

typedef struct {
    Texture2D spriteAtlas;
    Texture2D bg;

    Shader hitShader;

    Font font;

    Sound playerShot;
    Sound hit;
    Sound explosiveAsteroid;
    Sound pickUp;
    Sound select;
    Sound collideAsteroid;
    Sound engineLoop;

    Music bgm;
} Assets;

extern Game g;
extern Assets a;
extern Config c;

void Quit();

#endif
