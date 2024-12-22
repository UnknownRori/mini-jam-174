#pragma once

#ifndef SALVAGER_H
#define SALVAGER_H

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

void GameLoop(void);
void GameInit(void);
void GameUnload(void);
bool ShouldGameQuit(void);

#endif
