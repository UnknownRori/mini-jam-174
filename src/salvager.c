#include <raylib.h>
#include "./include/salvager.h"
#include "./include/types.h"
#include "./include/utils.h"

void GameLoop(void)
{
    BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(0, SCREEN_HEIGHT - 24);

    EndDrawing();
}
void GameInit(void)
{

}
void GameUnload(void)
{

}
