#include "./utils.h"
#include "include/logger.h"
#include "include/utils.h"
#include <raylib.h>
#include <raymath.h>


Time CalculateTime(f32 time)
{
    return (Time) {
        .minute = time / 60,
        .seconds = (int)time % 60
    };
}

bool CheckIfInGameSpace(Rectangle obj, Camera2D c, f32 screenWidth, f32 screenHeight)
{
    Rectangle camRect = (Rectangle){
        .x = c.target.x - screenWidth / 2.,
        .y = c.target.y - screenHeight / 2.,
        .width = screenWidth,
        .height = screenHeight
    };

    return !CheckCollisionRecs(camRect, obj);
}

Rectangle GetCameraBounds(Camera2D c, f32 screenWidth, f32 screenHeight)
{
    Rectangle bounds;

    f32 halfWidth = (screenWidth / 2.) / c.zoom;
    f32 halfHeight = (screenHeight / 2.) / c.zoom;

    bounds.x = c.target.x - halfWidth;
    bounds.y = c.target.x - halfHeight;
    bounds.width = halfWidth * 2.;
    bounds.height = halfHeight * 2.;

    return bounds;
}

void CameraShake(Camera2D *c, f32* shakeness, f32 recover)
{
    f32 shake = *shakeness;
    c->target = Vector2Add((Vector2) {
        .x = GetRandomValue(-shake * 10, shake * 10) / 10.,
        .y = GetRandomValue(-shake * 10, shake * 10) / 10.,
    }, c->target);
    *shakeness -= recover;
    *shakeness = MAX(0.f, *shakeness);
}
