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
#include "event.h"
#include "include/logger.h"
#include "include/timer.h"

#include "./ui.h"

static i32 select;

typedef struct 
{
    u32 id;
    Vector2 pos;
    char *text;
} SelectionButton;

Color SelectionButtonActiveColor(i32 currentIndex)
{
    if (currentIndex == select) return (Color) {128, 128, 128, 200};
    return (Color) {64, 64, 64, 200};
}

void SelectionButtonDraw(SelectionButton select)
{
    f32 fontSize = 24;
    f32 spacing = 4;
    Vector2 length = MeasureTextEx(a.font, select.text, fontSize, spacing);
    Rectangle rect = (Rectangle) {
        .x = select.pos.x + SCREEN_WIDTH * 0.01,
        .y = select.pos.y,
        .width = SCREEN_WIDTH * 0.98,
        .height = 80,
    };
    DrawRectanglePro(rect, VECTOR2_ZERO, 0, SelectionButtonActiveColor(select.id));
    DrawTextEx(a.font, select.text, (Vector2) {
        .x = SCREEN_WIDTH / 2. - length.x / 2.,
        .y = select.pos.y + length.y,
    }, 24, 4, WHITE);
}

void LevelupSelectionDraw(void)
{
    DrawTextEx(a.font, "Select power up", (Vector2) {180, 80}, 24, 4, WHITE);
    SelectionButtonDraw((SelectionButton) {
        .id = 0,
        .pos = (Vector2) {0, 200},
        .text = "Increased collection range"
    });
    SelectionButtonDraw((SelectionButton) {
        .id = 1,
        .pos = (Vector2) {0, 300},
        .text = "Fire rate"
    });
    SelectionButtonDraw((SelectionButton) {
        .id = 2,
        .pos = (Vector2) {0, 400},
        .text = "Multiply Drop rate"
    });
}
void LevelupSelectionUpdate(void)
{
    if (IsKeyPressed(KEY_UP)) select-= 1;
    if (IsKeyPressed(KEY_DOWN)) select+= 1;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_Z)) LevelUpPlayer(select);
    CLAMP(select, 0, 2);
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
