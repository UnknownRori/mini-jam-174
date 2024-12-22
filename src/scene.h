#pragma once
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

void SceneChange(int index);
void SceneAdd(int, Scene);
void SceneUpdate(void);
void SceneDraw(void);

#endif
