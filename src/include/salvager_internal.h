#pragma once

#ifndef SALVAGER_INTERNAL_H
#define SALVAGER_INTERNAL_H

#include <raylib.h>
#include "./salvager.h"
#include "./timer.h"
#include "./types.h"
#include "./utils.h"

#define ASTEROID_LIMIT 100
#define ASTEROID_LIFETIME 120
#define SCRAP_LIMIT 512

typedef struct {
    Vector2 position;
    Vector2 velocity;

    f32 acceleration;
    f32 friction;

    f32 maxSpeed;

    f32 rotation;
    f32 rotationSpeed;

    Timer engineBlink;
} Player;

typedef struct {
    Vector2 position;
    Vector2 velocity;

    f32 rotation;
    f32 rotationSpeed;

    Timer lifetime;
} Asteroid;

typedef struct {
    Vector2 position;
    u32 value;
} Scrap;

typedef struct {
    Player player;
    Asteroid asteroid[ASTEROID_LIMIT];
    Scrap scrap[SCRAP_LIMIT];

    Camera2D camera;
} Game;

typedef struct {
    u64 highscore;
    u64 score;

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
