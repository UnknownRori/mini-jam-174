#include "include/logger.h"
#include "include/salvager.h"
#include "include/timer.h"
#include "raylib.h"
#include "./scene.h"
#include "include/types.h"
#include "include/utils.h"

typedef enum
{
    CHANGE_NONE,
    CHANGE_FORWARD,
    CHANGE_BACKWARD
} SceneChangeType;

static u32 activeScene = 0;
static u32 nextScene = 0;
static Timer timeToChange = {0};
static SceneChangeType sceneChanging = CHANGE_NONE;

static Scene scenePtr[SCENE_MAX] = {0};

void SceneInit(Timer timeChange)
{
    timeToChange = timeChange;
}

void SceneSwap(int index)
{
    CLAMP(index, 0, SCENE_MAX);
    activeScene = index;
    scenePtr[activeScene].init();
}

void SceneChange(int index)
{
    CLAMP(index, 0, SCENE_MAX);
    TimerReset(&timeToChange);
    sceneChanging = CHANGE_FORWARD;
    nextScene = index;
}

void SceneAdd(int index, Scene scene)
{
    CLAMP(index, 0, SCENE_MAX);
    scenePtr[index] = scene;
}

void SceneDraw(void)
{
    scenePtr[activeScene].draw();
    if (sceneChanging) {
        f32 progress = 0.;
        if (sceneChanging == CHANGE_FORWARD) progress = 1. - TimeProgress(&timeToChange);
        if (sceneChanging == CHANGE_BACKWARD) progress = TimeProgress(&timeToChange);
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT * progress, BLACK);
    }
}

void SceneUpdate(void)
{
    if (sceneChanging) {
        TimerUpdate(&timeToChange);
        if (TimerCompleted(&timeToChange) && sceneChanging == CHANGE_FORWARD) {
            activeScene = nextScene;
            scenePtr[activeScene].init();
            sceneChanging = CHANGE_BACKWARD;
            TimerReset(&timeToChange);
        }
        if (TimerCompleted(&timeToChange) && sceneChanging == CHANGE_BACKWARD) {
            sceneChanging = CHANGE_NONE;
            TimerReset(&timeToChange);
        }
        return;
    }
    scenePtr[activeScene].update();
}
