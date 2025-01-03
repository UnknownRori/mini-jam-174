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
#include "utils.h"

#include "./ui.h"

static i32 selectLevelup = 0;
static i32 selectPauseMenu = 0;
static i32 selectMainMenu = 0;
static i32 selectOption = 0;

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
    DrawTextEx(a.font, "Game Paused", (Vector2) {250, 80}, 24, 4, WHITE);
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
    if (IsKeyPressed(KEY_ENTER) || IsKeyReleased(KEY_Z) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) PlaySound(a.select);
    if (IsKeyPressed(KEY_X)) g.paused = false;
    if (IsKeyPressed(KEY_ENTER) || IsKeyReleased(KEY_Z)) {
        switch (selectPauseMenu) {
            case 0:
                g.paused = false;
                break;
            case 1:
                SceneChange(SCENE_MAIN_MENU);
                break;
        }
    }
    CLAMP(selectPauseMenu, 0, 1);
}

void LevelupSelectionUpdate(void)
{
    if (g.playerCurrentlyShoot && IsKeyReleased(KEY_Z)) {g.playerCurrentlyShoot = false; return;}
    if (g.playerCurrentlyShoot) return;
    if (IsKeyPressed(KEY_UP)) selectLevelup-= 1;
    if (IsKeyPressed(KEY_DOWN)) selectLevelup+= 1;
    if (IsKeyPressed(KEY_ENTER) || IsKeyReleased(KEY_Z)) LevelUpPlayer(selectLevelup);
    if (IsKeyPressed(KEY_ENTER) || IsKeyReleased(KEY_Z) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) PlaySound(a.select);
    CLAMP(selectLevelup, 0, 2);
}

Color MainMenuSelectButtonColor(i32 currentIndex)
{
    if (currentIndex == selectMainMenu) return (Color) {128, 128, 128, 200};
    return (Color) {64, 64, 64, 200};
}

void SelectionMainMenuButton(SelectionButton select)
{
    SelectionButtonDraw(select, MainMenuSelectButtonColor(select.id));
}

void MainMenuSelectDraw(void)
{
    SelectionMainMenuButton((SelectionButton) {
        .id = 0,
        .pos = (Vector2) {0, 200},
        .text = "Start"
    });
    SelectionMainMenuButton((SelectionButton) {
        .id = 1,
        .pos = (Vector2) {0, 300},
        .text = "Options"
    });
    SelectionMainMenuButton((SelectionButton) {
        .id = 2,
        .pos = (Vector2) {0, 400},
        .text = "Exit"
    });
}
void MainMenuSelectUpdate(void)
{
    if (IsKeyPressed(KEY_UP)) selectMainMenu-= 1;
    if (IsKeyPressed(KEY_DOWN)) selectMainMenu+= 1;
    if (IsKeyPressed(KEY_ENTER) || IsKeyReleased(KEY_Z) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) PlaySound(a.select);
    if (IsKeyPressed(KEY_ENTER) || IsKeyReleased(KEY_Z)) {
        switch(selectMainMenu) {
            case 0:
                SceneChange(SCENE_GAME);
                break;
            case 1:
                SceneChange(SCENE_OPTION);
                break;
            case 2:
                Quit();
                break;
        }
    }
    CLAMP(selectMainMenu, 0, 2);
}

void ResourceDraw(void)
{
    float offset = 0;
    char buffer[512] ={0};

    if (TimerCompleted(&g.player.hitShaderDelay)) offset = GetRandomValue(2, 4);
    Rectangle dst = (Rectangle) {
        .x = 18 + offset,
        .y = 24 + offset,
        .width = SCRAP_SPRITE.width + 12,
        .height = SCRAP_SPRITE.height + 12,
    };
    Vector2 origin = (Vector2) {
        .x = SCRAP_SPRITE.width / 2.,
        .y = SCRAP_SPRITE.width / 2.,
    };

    snprintf(buffer, sizeof(buffer), "%.0lf", g.scrap_collected - g.scrap_spent);

    if (TimerCompleted(&g.player.hitShaderDelay))
    {
        BeginShaderMode(a.hitShader);
        DrawTexturePro(a.spriteAtlas, SCRAP_SPRITE, dst, origin, 0, WHITE);
        EndShaderMode();
        DrawTextEx(a.font, buffer, (Vector2) {SCRAP_SPRITE.width + 24, SCRAP_SPRITE.height / 2. + 12}, 24, 4, RED);
    }
    else 
    {
        DrawTexturePro(a.spriteAtlas, SCRAP_SPRITE, dst, origin, 0, WHITE);
        DrawTextEx(a.font, buffer, (Vector2) {SCRAP_SPRITE.width + 24, SCRAP_SPRITE.height / 2. + 12}, 24, 4, WHITE);
    }

    dst = (Rectangle) {
        .x = 18,
        .y = 64,
        .width = TIME_SPRITE.width + 12,
        .height = TIME_SPRITE.height + 12,
    };
    origin = (Vector2) {
        .x = TIME_SPRITE.width / 2.,
        .y = TIME_SPRITE.width / 2.,
    };
    DrawTexturePro(a.spriteAtlas, TIME_SPRITE, dst, origin, 0, WHITE);


    Time time = CalculateTime(g.elapsed);
    snprintf(buffer, sizeof(buffer), "%02d:%02d", time.minute, time.seconds);
    DrawTextEx(a.font, buffer, (Vector2) {32 + 24, 64}, 24, 4, WHITE);
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

void OptionSlider(i32 id, const char *text, Vector2 pos, f32 current, f32 max)
{
    if (id == selectOption) DrawTextEx(a.font, text, pos, 18, 4, WHITE);
    else  DrawTextEx(a.font, text, pos, 18, 4, (Color) {150, 150, 150, 255});

    char buffer[128] = {0};
    snprintf(buffer, sizeof(buffer), "%.0f", current * 10);
    DrawTextEx(a.font, buffer, Vector2Add(pos, (Vector2) {600, 50}), 18, 4, (Color) {150, 150, 150, 255});
    ProgressDraw((Rectangle) {
        .x = 80,
        .y = pos.y + 30,
        .width = SCREEN_WIDTH * 0.8,
        .height = 20,
    }, current, max, WHITE, (Color) {128, 128, 128, 255});
}

void ToggleButton(const char *text, Vector2 pos, bool shouldActive, Color active, Color deactive)
{
    if (shouldActive) 
        DrawTextEx(a.font, text, pos, 18, 4, WHITE);
    else
        DrawTextEx(a.font, text, pos, 18, 4, deactive);

    if (!c.fullscreen) {
        DrawRectangle(40, pos.y, 20, 20, WHITE);
        DrawRectangle(43, pos.y + 2, 15, 15, deactive);
    } else {
        DrawRectangle(40, pos.y, 20, 20, WHITE);
    }
    
}

void ToggleButtonOption(u32 id, const char *text, Vector2 pos)
{
    ToggleButton(text, pos, selectOption == id, (Color) {255, 255, 255, 255}, (Color) {150,150,150, 255});
}

void OptionSliderMenuDraw(void)
{
    DrawTextEx(a.font, "Options", (Vector2) {280, 80}, 24, 4, WHITE);
    OptionSlider(0, "BGM", (Vector2) {70., 150}, c.bgmVolume, 1.);
    OptionSlider(1, "SFX", (Vector2) {70., 250}, c.sfxVolume, 1.);
    OptionSlider(2, "Screen Shake", (Vector2) {70., 350}, c.shaking, 10.);
    ToggleButtonOption(3, "Fullscreen", (Vector2){70., 450});
}

void OptionSliderMenuUpdate(void)
{
    if (IsKeyPressed(KEY_UP)) selectOption -= 1;
    if (IsKeyPressed(KEY_DOWN)) selectOption += 1;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) PlaySound(a.select);
    switch(selectOption) {
        case 0:
            if (IsKeyPressed(KEY_LEFT)) c.bgmVolume -= 0.1;
            if (IsKeyPressed(KEY_RIGHT)) c.bgmVolume += 0.1;
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                SetSoundVolume(a.explosiveAsteroid, c.bgmVolume);
                SetMusicVolume(a.bgm, c.bgmVolume);
                PlaySound(a.explosiveAsteroid);
            }
            CLAMP(c.bgmVolume, 0, 1.);
            break;
        case 1:
            if (IsKeyPressed(KEY_LEFT)) c.sfxVolume -= 0.1;
            if (IsKeyPressed(KEY_RIGHT)) c.sfxVolume += 0.1;
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                SetSoundVolume(a.explosiveAsteroid, c.sfxVolume);
                PlaySound(a.explosiveAsteroid);
            }
            CLAMP(c.bgmVolume, 0, 1.);
            break;
        case 2:
            if (IsKeyPressed(KEY_LEFT)) c.shaking -= 1.;
            if (IsKeyPressed(KEY_RIGHT)) c.shaking += 1.;
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) {
                c.shakeness = c.shaking;
                PlaySound(a.explosiveAsteroid);
            }
            CLAMP(c.shaking, 0, 10.);
            break;
        case 3:
            if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) {
                c.fullscreen = !c.fullscreen;
                ToggleFullscreen();
            }
            break;
    }
    CLAMP(selectOption, 0, 3);
}

void GameOverMenuUpdate(void)
{
    if (IsKeyPressed(KEY_Z) || IsKeyPressed(KEY_ENTER)) SceneChange(SCENE_MAIN_MENU);
}
void GameOverMenuDraw(void)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color) {0, 0, 0, 200});
    DrawTextEx(a.font, "Game Over", (Vector2) {200, 150}, 42, 4, WHITE);
    DrawTextEx(a.font, "Good luck on next attempt", (Vector2) {120, 250}, 18, 4, WHITE);
    DrawTextEx(a.font, "Press Accept to Continue", (Vector2) {140, 500}, 18, 4, WHITE);

    char buffer[512] = {0};
    snprintf(buffer, sizeof(buffer), "%.0lf", g.scrap_collected);
    DrawTextEx(a.font, buffer, (Vector2) {350, 350}, 16, 4, WHITE);

    Time time = CalculateTime(g.elapsed);
    snprintf(buffer, sizeof(buffer), "%02d:%02d", time.minute, time.seconds);
    DrawTextEx(a.font, buffer, (Vector2) {350, 400}, 16, 4, WHITE);

    Rectangle dst = (Rectangle) {
        .x = 300,
        .y = 350,
        .width = SCRAP_SPRITE.width + 12,
        .height = SCRAP_SPRITE.height + 12,
    };
    Vector2 origin = (Vector2) {
        .x = SCRAP_SPRITE.width / 2.,
        .y = SCRAP_SPRITE.width / 2.,
    };
    DrawTexturePro(a.spriteAtlas, SCRAP_SPRITE, dst, origin, 0, WHITE);

    dst = (Rectangle) {
        .x = 300,
        .y = 400,
        .width = TIME_SPRITE.width + 12,
        .height = TIME_SPRITE.height + 12,
    };
    origin = (Vector2) {
        .x = TIME_SPRITE.width / 2.,
        .y = TIME_SPRITE.width / 2.,
    };
    DrawTexturePro(a.spriteAtlas, TIME_SPRITE, dst, origin, 0, WHITE);
}
