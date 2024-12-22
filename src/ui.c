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
#include "scene.h"

#include "./ui.h"

static i32 selectLevelup;
static i32 selectPauseMenu;

typedef struct 
{
    u32 id;
    Vector2 pos;
    char *text;
} SelectionButton;

static Color SelectionButtonPauseActiveColor(i32 currentIndex)
{
    if (currentIndex == selectPauseMenu) return (Color) {128, 128, 128, 200};
    return (Color) {64, 64, 64, 200};
}

static Color SelectionButtonLevelUpActiveColor(i32 currentIndex)
{
    if (currentIndex == selectLevelup) return (Color) {128, 128, 128, 200};
    return (Color) {64, 64, 64, 200};
}


void SelectionButtonDraw(SelectionButton select, Color color)
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
    DrawRectanglePro(rect, VECTOR2_ZERO, 0, color);
    DrawTextEx(a.font, select.text, (Vector2) {
        .x = SCREEN_WIDTH / 2. - length.x / 2.,
        .y = select.pos.y + length.y,
    }, 24, 4, WHITE);
}

void SelectionPauseButton(SelectionButton select)
{
    SelectionButtonDraw(select, SelectionButtonPauseActiveColor(select.id));
}

void SelectionLevelUpButton(SelectionButton select)
{
    SelectionButtonDraw(select, SelectionButtonLevelUpActiveColor(select.id));
}

void PauseSelectDraw(void)
{
    DrawTextEx(a.font, "Game Paused", (Vector2) {160, 80}, 24, 4, WHITE);
    SelectionPauseButton((SelectionButton) {
        .id = 0,
        .pos = (Vector2) {0, 200},
        .text = "Continue"
    });
    SelectionPauseButton((SelectionButton) {
        .id = 1,
        .pos = (Vector2) {0, 300},
        .text = "Exit"
    });
}

void LevelupSelectionDraw(void)
{
    DrawTextEx(a.font, "Select power up", (Vector2) {180, 80}, 24, 4, WHITE);
    SelectionLevelUpButton((SelectionButton) {
        .id = 0,
        .pos = (Vector2) {0, 200},
        .text = "Increased Collection Range"
    });
    SelectionLevelUpButton((SelectionButton) {
        .id = 1,
        .pos = (Vector2) {0, 300},
        .text = "Fire rate"
    });
    SelectionLevelUpButton((SelectionButton) {
        .id = 2,
        .pos = (Vector2) {0, 400},
        .text = "Multiply Drop rate"
    });
}

void PauseSelectUpdate(void)
{
    if (IsKeyPressed(KEY_UP)) selectPauseMenu-= 1;
    if (IsKeyPressed(KEY_DOWN)) selectPauseMenu+= 1;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_Z)) {
        switch (selectPauseMenu) {
            case 0:
                g.paused = false;
                break;
            case 1:
                SceneChange(0);
                break;
        }
    }
    CLAMP(selectPauseMenu, 0, 1);
}

void LevelupSelectionUpdate(void)
{
    if (IsKeyPressed(KEY_UP)) selectLevelup-= 1;
    if (IsKeyPressed(KEY_DOWN)) selectLevelup+= 1;
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_Z)) LevelUpPlayer(selectLevelup);
    CLAMP(selectLevelup, 0, 2);
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
