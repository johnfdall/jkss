#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

typedef enum { 
        CMD_MOVE, 
        CMD_ATTACK, 
} CommandType;

typedef struct Point {
        int32_t x;
        int32_t y;
} Point;

#endif
