#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef __cplusplus

#include <stdio.h>
#include <stdlib.h>

#else

#include <cstdlib>
int fprintf();

#endif

#define ISSUE_ERROR(fmt, ...) { \
    fprintf(stderr, (fmt"\n")__VA_OPT__(,) __VA_ARGS__); \
    exit(-1); \
}

#endif //__COMMON_H__