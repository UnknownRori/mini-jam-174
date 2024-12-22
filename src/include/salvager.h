#pragma once

#ifndef SALVAGER_H
#define SALVAGER_H

#include <stdbool.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define FULLSCREEN 0

void GameLoop(void);
void GameInit(void);
void GameUnload(void);
bool ShouldGameQuit(void);

#endif
