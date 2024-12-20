#include <raylib.h>
#include "./include/salvager.h"


#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void UpdateGame(void);

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Salvager");

    InitAudioDevice();
    GameInit();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateGame, 0, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
      UpdateGame();
    }
#endif

    GameUnload();
    CloseWindow();

    return 0;
}

void UpdateGame(void) {
  GameLoop();
}
