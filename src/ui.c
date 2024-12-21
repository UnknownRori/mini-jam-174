#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "./include/salvager.h"
#include "./include/salvager_internal.h"
#include "./include/types.h"
#include "./include/utils.h"
#include "include/logger.h"
#include "include/timer.h"

#include "./ui.h"

void LevelupSelectionDraw(void)
{

}
void LevelupSelectionUpdate(void)
{

}

void ResourceDraw(void)
{
    Rectangle dst = (Rectangle) {
        .x = 18,
        .y = 24,
        .width = SCRAP_SPRITE.width + 12,
        .height = SCRAP_SPRITE.height + 12,
    };
    Vector2 origin = (Vector2) {
        .x = SCRAP_SPRITE.width / 2.,
        .y = SCRAP_SPRITE.width / 2.,
    };
    DrawTexturePro(a.spriteAtlas, SCRAP_SPRITE, dst, origin, 0, WHITE);

    char buffer[512] ={0};
    snprintf(buffer, sizeof(buffer), "%.0lf", g.scrap_collected - g.scrap_spent);

    DrawTextEx(a.font, buffer, (Vector2) {SCRAP_SPRITE.width + 24, SCRAP_SPRITE.height / 2. + 12}, 24, 4, WHITE);
}

void ProgressDraw(Rectangle size, f32 current, f32 max, Color active, Color deactive)
{
    f32 currentPercentage = current / max;
    CLAMP(currentPercentage, 0, 1);
    float activeWidth = (size.width * currentPercentage);


    DrawRectangleRec(size, deactive);
    DrawRectangleRec((Rectangle) {
        .x = size.x, 
        .y = size.y, 
        .width = activeWidth, 
        .height = size.height
    }, active);
}
