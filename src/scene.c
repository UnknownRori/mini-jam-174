#include "raylib.h"
#include "./scene.h"
#include "include/types.h"
#include "include/utils.h"

static u32 activeScene = 0;
static Scene scenePtr[SCENE_MAX] = {0};

void SceneChange(int index)
{
    CLAMP(index, 0, SCENE_MAX);
    activeScene = index;
    scenePtr[activeScene].init();
}

void SceneAdd(int index, Scene scene)
{
    CLAMP(index, 0, SCENE_MAX);
    scenePtr[index] = scene;
}

void SceneDraw(void)
{
    scenePtr[activeScene].draw();
}

void SceneUpdate(void)
{
    scenePtr[activeScene].update();
}
