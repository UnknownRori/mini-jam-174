#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/*#define DEBUG*/

#ifdef DEBUG
#define __LOG(format, ...) printf("\e[1m[ LOG ] | %s (%d): "format" \e[0m\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define __ERROR(format, ...) printf("\e[1;41m[ERROR]\e[0m | \e[1m%s (%d): "format" \e[0m\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#else
#define __LOG(format, ...) ((void)0)
#define __ERROR(format, ...) ((void)0)
#endif

#endif
