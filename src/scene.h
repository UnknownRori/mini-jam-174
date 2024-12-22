#pragma once
#include "include/timer.h"
#ifndef SCENE_H
#define SCENE_H

#define SCENE_MAX 5

typedef void (*SceneFunction)(void);

typedef struct
{
    SceneFunction init;
    SceneFunction draw;
    SceneFunction update;
} Scene;

void SceneInit(Timer timeChange);
void SceneChange(int index);
void SceneSwap(int index);
void SceneAdd(int, Scene);
void SceneUpdate(void);
void SceneDraw(void);

#endif
