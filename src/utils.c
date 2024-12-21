#include "./utils.h"
#include <raylib.h>


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
