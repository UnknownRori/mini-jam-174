#include <raylib.h>
#include "include/salvager_internal.h"
#include <math.h>
#include "paralax.h"
void ParalaxDraw(void)
{
    const Vector2 cameraPos = g.camera.target;
    /*BeginBlendMode(BLEND_ALPHA);*/
    for (u32 i = 0; i < PARALAX_MAX; i++)
    {
        const Paralax *paralax = &g.paralax[i];
        f32 wrappedX = fmodf(cameraPos.x * paralax->factor, paralax->img.width);
        f32 wrappedY = fmodf(cameraPos.y * paralax->factor, paralax->img.height);
        if (wrappedX < 0) wrappedX += paralax->img.width;
        if (wrappedY < 0) wrappedX += paralax->img.height;
        for (i32 x = PARALAX_MIN; x <= SCREEN_WIDTH / paralax->img.width + 2; x++)
        {
            for (i32 y = PARALAX_MIN; y < SCREEN_HEIGHT / paralax->img.height + 2; y++)
            {
                DrawTexture(
                    paralax->img, 
                    wrappedX + x * paralax->img.width, 
                    wrappedY + y * paralax->img.height, 
                    WHITE
                );
            }
        }
    }
    /*EndBlendMode();*/
}
