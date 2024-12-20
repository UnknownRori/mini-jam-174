#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define DEBUG

#ifdef DEBUG
#define __LOG(message) printf("\e[1m[ LOG ] | %s (%d): %s \e[0m\n", __FILENAME__, __LINE__, message)
#define __ERROR(message) printf("\e[1;41m[ERROR]\e[0m | \e[1m%s (%d): %s \e[0m\n", __FILENAME__, __LINE__, message)
#else
#define __LOG(message)
#define __ERROR(message)
#endif

#endif
