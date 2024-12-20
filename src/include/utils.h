#pragma once

#ifndef UTILS_H
#define UTILS_H

#define ARR_LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))
#define ARR2D(ARR, ROWS, I, J) ((I) * (ROWS) + J)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define CLAMP(x, min, max) {     \
    if(x < min) x += max;        \
    if(x >= min) x -= max;       \
}

#define VECTOR2_ZERO ((Vector2){0,0})

#endif
