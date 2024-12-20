#pragma once

#ifndef SALVAGER_INTERNAL_H
#define SALVAGER_INTERNAL_H

#include <raylib.h>
#include "./salvager.h"
#include "./timer.h"
#include "./types.h"
#include "./utils.h"

#define ASTEROID_LIMIT 100
#define ASTEROID_LIFETIME 20

#define SCRAP_LIMIT 512
#define SCRAP_LIFETIME 15

#define PARALAX_LIMIT 1
#define PARALAX_MIN -2
#define PARALAX_MAX 1

#define LEVEL_REQUIREMENT 100
#define LEVEL_MULTIPLIER 2

typedef struct {
    Vector2 position;
    Vector2 velocity;

    f32 acceleration;
    f32 friction;

    f32 maxSpeed;

    f32 rotation;
    f32 rotationSpeed;

    f32 collecionRadius;

    Timer engineBlink;
} Player;

typedef struct {
    Vector2 position;
    Vector2 velocity;

    f32 rotation;
    f32 rotationSpeed;

    Timer lifetime;
    bool active;
} Asteroid;

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

typedef struct {
    Player player;
    Asteroid asteroid[ASTEROID_LIMIT];
    Scrap scrap[SCRAP_LIMIT];

    u64 highscore;
    u64 score;
    f32 scrap_collected;
    f32 scrap_spent;
    Paralax paralax[PARALAX_LIMIT];

    Camera2D camera;
} Game;

typedef struct {
    f32 sfxVolume;
    f32 bgmVolume;
} Config;

#define PLAYER_SPRITE ((Rectangle) {0, 0, 32, 32})
#define ENGINE_EXHAUST_SPRITE ((Rectangle) {160, 0, 32, 32})
#define SCRAP_SPRITE ((Rectangle) {192, 0, 32, 32})
#define ASTEROID_SPRITE1 ((Rectangle) {0, 64, 32, 32})
#define ASTEROID_SPRITE2 ((Rectangle) {32, 64, 32, 32})

typedef struct {
    Texture2D spriteAtlas;
    Texture2D bg;

    Font font;

    Sound shoot;
    Sound pickUp;
    Music bgm;
} Assets;

#endif
